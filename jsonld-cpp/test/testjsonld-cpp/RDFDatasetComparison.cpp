#include "RDFDatasetComparison.h"

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>

#include <jsonld-cpp/RDFQuad.h>


namespace {

    using namespace RDF;

    bool compareGraphName(const std::shared_ptr<Node>& name1, const std::shared_ptr<Node>& name2, std::map<std::string, std::string> *mapping) {

        if (!name1 && !name2) {
            return true;
        }

        if (!name1 || !name2) {
            return false;
        }

        if (name1->isBlankNode() && name2->isBlankNode()) {
            return name1->getValue() ==
                   (mapping != nullptr ? mapping->at(name2->getValue()) : name2->getValue());

        } else if (name1->isIRI() && name2->isIRI()) {
            return name1->getValue() == name2->getValue();
        }
        return false;
    }

    bool compareObject(const std::shared_ptr<Node>& object1, const std::shared_ptr<Node>& object2, std::map<std::string, std::string> *mapping) {
        if(object1->isBlankNode() && object2->isBlankNode()) {
            return object1->getValue() ==
                   (mapping != nullptr ? mapping->at(object2->getValue()) : object2->getValue());
        }
        else if((object1->isIRI() && object2->isIRI()) || (object1->isLiteral() && object2->isLiteral())) {
            return object1->getValue() == object2->getValue();
        }
        return false;
    }

    bool comparePredicate(const std::shared_ptr<Node>& predicate1, const std::shared_ptr<Node>& predicate2) {
        return predicate1->getValue() == predicate2->getValue();
    }

    bool compareSubject(const std::shared_ptr<Node>& subject1, const std::shared_ptr<Node>& subject2, std::map<std::string, std::string> *mapping) {
        if(subject1->isBlankNode() && subject2->isBlankNode()) {
            return subject1->getValue() ==
                   (mapping != nullptr ? mapping->at(subject2->getValue()) : subject2->getValue());
        }
        else if(subject1->isIRI() && subject2->isIRI()) {
            return subject1->getValue() == subject2->getValue();
        }
        return false;
    }

    bool compareNQuad(RDFQuad &nquad1, RDFQuad &nquad2, std::map<std::string, std::string> *mapping) {
        return
                compareSubject(nquad1.getSubject(), nquad2.getSubject(), mapping) &&
                comparePredicate(nquad1.getPredicate(), nquad2.getPredicate()) &&
                compareObject(nquad1.getObject(), nquad2.getObject(), mapping) &&
                compareGraphName(nquad1.getGraph(), nquad2.getGraph(), mapping);
    }

    bool
    compareNQuads(
            const std::vector<RDFQuad>& nquads1,
            const std::vector<RDFQuad>& nquads2,
            std::map<std::string, std::string> *mapping) {

        std::vector<RDFQuad> remaining(nquads2);

        // foreach quad in first collection
        for (RDFQuad nquad1 : nquads1) {

            bool found = false;

            // see if second collection has a matching quad
            for (auto nquad2_it = remaining.begin(); nquad2_it != remaining.end(); ) {
                found = compareNQuad(*nquad2_it, nquad1, mapping);

                if (found) {
                    remaining.erase(nquad2_it);
                    break;
                } else {
                    ++nquad2_it;
                }
            }

            if (!found) {
                return false;
            }
        }
        return remaining.empty();
    }

    class NodeCategory {
    private:
        int objectCount = 0;
        int subjectCount = 0;
        int graphCount = 0;

        std::set<std::string> names;

    public:
        void addObject(const std::string & object);

        void addObject();

        void addSubject(const std::string & subject);

        void addSubject();

        void addGraph();

        bool operator<(const NodeCategory &rhs) const;
    };

    class NodeCluster {
    private:
        std::map<std::string, std::size_t> source_;

        std::vector<std::string> target_;
        std::vector<std::size_t> indices_;
        std::vector<std::size_t> mapping_;

        std::size_t iterator_{};
        int numPermutations_;

        void init();
        static int factorial(int number);

    public:
        NodeCluster(
                std::map<std::string, std::size_t> source,
                std::vector<std::string> target);

        static std::shared_ptr<NodeCluster> create(const std::vector<std::string>& source, const std::vector<std::string>& target);

        bool next();

        std::string mapping(const std::string& label) const;

        int numPermutations() const;

    };

    class NodeClassifier {
    private:
        std::map<std::string, NodeCategory> categories;

        void ensureCategoryForLabel(const std::string &label);

        void addSubject(const std::string &label, const std::string &object);

        void addSubject(const std::string &label);

        void addObject(const std::string &label, const std::string &object);

        void addObject(const std::string &label);

        void addGraph(const std::string &label);

    public:
        void add(const RDFQuad &nquad);

        using size_type = std::map<std::string, NodeCategory>::size_type;

        size_type size();

        std::map<NodeCategory, std::vector<std::string>> reduce();

    };

    class NodeMapper {
    private:
        std::map<std::string, std::shared_ptr<NodeCluster>> mapping_;
        int permutations_;
        int iterator_;

        NodeMapper();
        NodeMapper(int permutations, std::map<std::string, std::shared_ptr<NodeCluster>> mapping);

        static std::unique_ptr<std::map<std::string, std::shared_ptr<NodeCluster>>> merge(
                const std::map<NodeCategory, std::vector<std::string>>& reducedSource,
                const std::map<NodeCategory, std::vector<std::string>>& reducedTarget,
                int & permutations);

        std::vector<NodeCluster*> getClusters();

    public:
        static NodeMapper create(const std::vector<RDFQuad>& from, const std::vector<RDFQuad>& to);
        int permutations() const;
        bool hasNext() const;
        std::map<std::string, std::string> next();
    };

    void NodeCategory::addObject(const std::string &object) {
        objectCount++;
        names.insert(object);
    }

    void NodeCategory::addObject() {
        objectCount++;
    }

    void NodeCategory::addSubject(const std::string &subject) {
        subjectCount++;
        names.insert(subject);
    }

    void NodeCategory::addSubject() {
        subjectCount++;
    }

    void NodeCategory::addGraph() {
        graphCount++;
    }

    bool NodeCategory::operator<(const NodeCategory &rhs) const {
        return std::tie(objectCount, subjectCount, graphCount, names) <
               std::tie(rhs.objectCount, rhs.subjectCount, rhs.graphCount, rhs.names);
    }

    NodeCluster::NodeCluster(std::map<std::string, std::size_t> source, std::vector<std::string> target)
            : source_(std::move(source)), target_(std::move(target)), iterator_(0) {
        numPermutations_ = factorial(static_cast<int>(source_.size()));
        init();
    }

    void NodeCluster::init() {
        indices_.assign(source_.size(), 0);
        mapping_.assign(source_.size(), 0);
        std::iota(mapping_.begin(), mapping_.end(), 0);
        iterator_ = 0;
    }

    std::shared_ptr<NodeCluster> NodeCluster::create(const std::vector<std::string> &source, const std::vector<std::string> &target) {

        if (source.size() != target.size()) {
            throw std::invalid_argument("source.size() != target.size() in NodeCluster");
        }

        std::map<std::string, std::size_t> sourceMap;
        for (const auto& s : source) {
            sourceMap.insert(std::make_pair(s, sourceMap.size()));
        }

        return std::make_shared<NodeCluster>(sourceMap, target);
    }

    int NodeCluster::factorial(int number) {
        int result = 1;
        for(int i=1; i <= number; i++){
            result *= i;
        }
        return result;
    }

    bool NodeCluster::next() {

        if (indices_.size() == 1) {
            return true;
        }

        while (iterator_ < indices_.size()) {
            if (indices_[iterator_] < iterator_) {
                std::swap(
                        mapping_[(iterator_ % 2 == 0 ? 0 : indices_[iterator_])],
                        mapping_[iterator_]);
                indices_[iterator_]++;
                iterator_ = 0;
                break;
            }
            else {
                indices_[iterator_] = 0;
                iterator_++;
            }
        }

        if (iterator_ >= indices_.size()) {
            init();
            return true;
        }
        return false;
    }

    std::string NodeCluster::mapping(const std::string &label) const {
        return target_[mapping_[source_.at(label)]];
    }

    int NodeCluster::numPermutations() const {
        return numPermutations_;
    }

    void NodeClassifier::ensureCategoryForLabel(const std::string &label) {
        if (!categories.count(label)) {
            NodeCategory nodeCategory;
            categories[label] = nodeCategory;
        }
    }

    void NodeClassifier::addSubject(const std::string &label, const std::string &object) {
        ensureCategoryForLabel(label);
        categories[label].addSubject(object);
    }

    void NodeClassifier::addSubject(const std::string &label) {
        ensureCategoryForLabel(label);
        categories[label].addSubject();
    }

    void NodeClassifier::addObject(const std::string &label, const std::string &object) {
        ensureCategoryForLabel(label);
        categories[label].addObject(object);
    }

    void NodeClassifier::addObject(const std::string &label) {
        ensureCategoryForLabel(label);
        categories[label].addObject();
    }

    void NodeClassifier::addGraph(const std::string &label) {
        ensureCategoryForLabel(label);
        categories[label].addGraph();
    }

    void NodeClassifier::add(const RDFQuad &nquad) {

        if (nquad.getSubject()->isBlankNode()) {
            if (nquad.getObject()->isIRI())
                addSubject(nquad.getSubject()->getValue(), nquad.getObject()->getValue());
            else
                addSubject(nquad.getSubject()->getValue());
        }
        if (nquad.getObject()->isBlankNode()) {
            if (nquad.getSubject()->isIRI())
                addObject(nquad.getObject()->getValue(), nquad.getSubject()->getValue());
            else
                addObject(nquad.getObject()->getValue());
        }
        if (nquad.getGraph() && nquad.getGraph()->isBlankNode()) {
            addGraph(nquad.getGraph()->getValue());
        }
    }

    NodeClassifier::size_type NodeClassifier::size() {
        return categories.size();
    }

    std::map<NodeCategory, std::vector<std::string>> NodeClassifier::reduce() {

        std::map<NodeCategory, std::vector<std::string>> reduced;

        for (const auto &element : categories) {
            const std::string &label = element.first;
            const NodeCategory &nodeCategory = element.second;
            if (reduced.count(nodeCategory)) {
                reduced[nodeCategory].push_back(label);
            } else {
                std::vector<std::string> labels;
                labels.push_back(label);
                reduced.insert(std::make_pair(nodeCategory, labels));
            }
        }

        return reduced;
    }

    NodeMapper::NodeMapper()
            : permutations_(-1), iterator_(0)
    { }

    NodeMapper::NodeMapper(int permutations, std::map<std::string, std::shared_ptr<NodeCluster>> mapping)
            : mapping_(mapping), permutations_(permutations), iterator_(0)
    { }

    std::unique_ptr<std::map<std::string, std::shared_ptr<NodeCluster>>>
    NodeMapper::merge(const std::map<NodeCategory, std::vector<std::string>> &reducedSource,
                      const std::map<NodeCategory, std::vector<std::string>> &reducedTarget,
                      int &permutations) {

        if (reducedSource.size() != reducedTarget.size()) {
            return nullptr;
        }

        std::unique_ptr<std::map<std::string, std::shared_ptr<NodeCluster>>> mapping(new std::map<std::string, std::shared_ptr<NodeCluster>>());

        permutations = 1;

        for (const auto &element : reducedSource) {

            if(reducedTarget.count(element.first) == 0)
                return nullptr;

            const std::vector<std::string>& target = reducedTarget.at(element.first);

            if (target.size() != element.second.size()) {
                return nullptr;
            }

            std::shared_ptr<NodeCluster> pcluster = NodeCluster::create(element.second, target);
            permutations *= pcluster->numPermutations();
            for(const auto &l : element.second)
                mapping->insert(std::make_pair(l, pcluster));
        }

        return mapping;

    }

    int NodeMapper::permutations() const {
        return permutations_;
    }

    bool NodeMapper::hasNext() const {
        return iterator_ != -1 && iterator_ < permutations_;
    }

    std::map<std::string, std::string> NodeMapper::next() {

        if (!hasNext()) {
            throw std::invalid_argument("!hasNext() in NodeMapper");
        }

        if (iterator_ > 0) {
            for (NodeCluster *cluster : getClusters()) {
                if (!cluster->next()) {
                    break;
                }
            }
        }

        std::map<std::string, std::string> result;

        for (const auto &element : mapping_) {
            result.insert(std::make_pair(element.first, element.second->mapping(element.first)));
        }

        iterator_++;

        return result;
    }

    std::vector<NodeCluster*> NodeMapper::getClusters() {
        std::set<NodeCluster*> s;
        for( auto& nc : mapping_) {
            s.insert(nc.second.get());
        }
        return {s.begin(), s.end()};
    }

    NodeMapper NodeMapper::create(const std::vector<RDFQuad> &from, const std::vector<RDFQuad> &to) {

        NodeClassifier sourceClassifier;
        for(const auto &quad : to) {
            sourceClassifier.add(quad);
        }

        NodeClassifier targetClassifier;
        for(const auto &quad : from) {
            targetClassifier.add(quad);
        }

        // cannot create mapping if blank nodes count do not match
        if (sourceClassifier.size() != targetClassifier.size()) {
            return {};
        }

        auto reducedSource = sourceClassifier.reduce();

        auto reducedTarget = targetClassifier.reduce();

        int permutations = 0;
        auto mapping = merge(reducedSource, reducedTarget, permutations);
        if(!mapping)
            return {};

        return {permutations, *mapping};
    }

}


namespace RDF {

    bool areIsomorphic(const RDFDataset & dataset1, const RDFDataset & dataset2) {

        // if datasets are empty
        if (dataset1.empty() && dataset2.empty()) {
            return true;
        }

        // compare total number of n-quads
        if (dataset1.numTriples() != dataset2.numTriples()) {
            return false;
        }

        // convert dataset's "graphnames + triples" to quads for easier processing
        std::vector<RDFQuad> nquads1 = dataset1.getAllGraphsAsQuads();
        std::vector<RDFQuad> nquads2 = dataset2.getAllGraphsAsQuads();

        std::vector<RDFQuad> nquads1_noBlanks;
        std::copy_if(nquads1.begin(), nquads1.end(), std::back_inserter(nquads1_noBlanks),
                     [](const RDFQuad& q) {
                         return !(q.getObject()->isBlankNode() ||
                                  q.getSubject()->isBlankNode() ||
                                  (q.getGraph() && q.getGraph()->isBlankNode()));
                     });

        std::vector<RDFQuad> nquads2_noBlanks;
        std::copy_if(nquads2.begin(), nquads2.end(), std::back_inserter(nquads2_noBlanks),
                     [](const RDFQuad& q) {
                         return !(q.getObject()->isBlankNode() ||
                                  q.getSubject()->isBlankNode() ||
                                  (q.getGraph() && q.getGraph()->isBlankNode()));
                     });

        // compare number of non-blank node n-quads
        if (nquads1_noBlanks.size() != nquads2_noBlanks.size()) {
            return false;
        }

        if (!compareNQuads(nquads1_noBlanks, nquads2_noBlanks, nullptr)) {
            return false;
        }

        // if datasets have no blank nodes
        if (nquads1_noBlanks.size() == dataset2.numTriples()) {
            return true;
        }

        std::vector<RDFQuad> nquads1_blanks;
        std::copy_if(nquads1.begin(), nquads1.end(), std::back_inserter(nquads1_blanks),
                     [](const RDFQuad& q) {
                         return q.getObject()->isBlankNode() ||
                                q.getSubject()->isBlankNode() ||
                                (q.getGraph() && q.getGraph()->isBlankNode());
                     });

        std::vector<RDFQuad> nquads2_blanks;
        std::copy_if(nquads2.begin(), nquads2.end(), std::back_inserter(nquads2_blanks),
                     [](const RDFQuad& q) {
                         return q.getObject()->isBlankNode() ||
                                q.getSubject()->isBlankNode() ||
                                (q.getGraph() && q.getGraph()->isBlankNode());
                     });

        // compare number of blank node n-quads
        if (nquads1_blanks.size() != nquads2_blanks.size()) {
            return false;
        }

        // create mappings from nquads2_blanks to nquads1_blanks
        NodeMapper mapper = NodeMapper::create(nquads2_blanks, nquads1_blanks);

        int iteration = 0;

        while (mapper.hasNext()) {

            std::map<std::string, std::string> mapping = mapper.next();
            if (compareNQuads(nquads1_blanks, nquads2_blanks, &mapping)) {
                return true;
            }

            iteration++;

            if (iteration >= 50) {
                std::cerr << "Too many iterations for permutations [" << iteration << ", " << mapper.permutations() << "]" << std::endl;
                return false;
            }
        }

        return false;
    }

}
