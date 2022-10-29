#include "jsonld-cpp/RDFDatasetComparison.h"
#include <set>
#include <iostream>

using nlohmann::json;

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

        for (RDFQuad nquad1 : nquads1) {

            bool found = false;

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

        std::set<std::string> objects;
        std::set<std::string> subjects;

    public:
        void addObject(const std::string & object);

        void addObject();

        void addSubject(const std::string & subject);

        void addSubject();

        void addGraph();

        bool operator<(const NodeCategory &rhs) const;

        bool operator>(const NodeCategory &rhs) const;

        bool operator<=(const NodeCategory &rhs) const;

        bool operator>=(const NodeCategory &rhs) const;

        bool operator==(const NodeCategory &rhs) const;

        bool operator!=(const NodeCategory &rhs) const;
    };

    class NodeCluster {
    private:
        std::map<std::string, std::size_t> source_;

        std::vector<std::string> target_;
        std::vector<std::size_t> indices_;
        std::vector<std::size_t> mapping_;

        std::size_t iterator_;
        int permutations_;

        void init();
        static int factorial(int number);
        void swap_mapping(std::size_t a, std::size_t b);

    public:
        NodeCluster(
                std::map<std::string, std::size_t> source,
                std::vector<std::string> target,
                int permutations);

        static NodeCluster create(const std::vector<std::string>& source, const std::vector<std::string>& target);


        bool next();

        std::string mapping(const std::string& label) const;

        int permutations() const;

    };

    class NodeClassifier {
    private:
        std::map<std::string, NodeCategory> categories;

        void addSubject(const std::string &label, const std::string &object);

        void addSubject(const std::string &label);

        void addObject(const std::string &label, const std::string &subject);

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
        std::map<std::string, NodeCluster> mapping_;
        std::vector<NodeCluster> clusters_;
        int permutations_;
        int iterator_;

        NodeMapper();

        NodeMapper(int permutations, std::map<std::string, NodeCluster> mapping, std::vector<NodeCluster> clusters);

        static std::unique_ptr<std::map<std::string, NodeCluster>> merge(
                const std::map<NodeCategory, std::vector<std::string>>& reducedSource,
                const std::map<NodeCategory, std::vector<std::string>>& reducedTarget,
                std::vector<NodeCluster> & clusters);

    public:

        int permutations() const;

        bool hasNext() const;

        std::map<std::string, std::string> next();

        static NodeMapper create(const std::vector<RDFQuad>& from, const std::vector<RDFQuad>& to);

    };

    void NodeCategory::addObject(const std::string &object) {
        objectCount++;
        objects.insert(object);
    }

    void NodeCategory::addObject() {
        objectCount++;
    }

    void NodeCategory::addSubject(const std::string &subject) {
        subjectCount++;
        objects.insert(subject);
    }

    void NodeCategory::addSubject() {
        subjectCount++;
    }

    void NodeCategory::addGraph() {
        graphCount++;
    }

    bool NodeCategory::operator<(const NodeCategory &rhs) const {
        return std::tie(objectCount, subjectCount, graphCount, objects, subjects) <
               std::tie(rhs.objectCount, rhs.subjectCount, rhs.graphCount, rhs.objects, rhs.subjects);
    }

    bool NodeCategory::operator>(const NodeCategory &rhs) const {
        return rhs < *this;
    }

    bool NodeCategory::operator<=(const NodeCategory &rhs) const {
        return !(rhs < *this);
    }

    bool NodeCategory::operator>=(const NodeCategory &rhs) const {
        return !(*this < rhs);
    }

    bool NodeCategory::operator==(const NodeCategory &rhs) const {
        return std::tie(objectCount, subjectCount, graphCount, objects, subjects) ==
               std::tie(rhs.objectCount, rhs.subjectCount, rhs.graphCount, rhs.objects, rhs.subjects);
    }

    bool NodeCategory::operator!=(const NodeCategory &rhs) const {
        return !(rhs == *this);
    }

    NodeCluster::NodeCluster(std::map<std::string, std::size_t> source, std::vector<std::string> target,
                             int permutations)
            : source_(std::move(source)), target_(std::move(target)), permutations_(permutations) {
        indices_.assign(source_.size(), 0);
        mapping_.assign(source_.size(), 0);
        iterator_ = 0;
        init();
    }

    void NodeCluster::init() {
        indices_.assign(indices_.size(), 0);
        std::iota(mapping_.begin(), mapping_.end(), 0);
        iterator_ = 0;
    }

    NodeCluster NodeCluster::create(const std::vector<std::string> &source, const std::vector<std::string> &target) {

        if (source.size() != target.size()) {
            throw std::invalid_argument("source.size() != target.size() in NodeCluster");
        }

        std::map<std::string, std::size_t> sourceMap;
        for (const auto& s : source) {
            sourceMap.insert(std::make_pair(s, sourceMap.size()));
        }

        return {sourceMap, target, factorial(static_cast<int>(source.size()))};
    }

    int NodeCluster::factorial(int number) {
        int result = 1;
        for(int i=1; i <= number; i++){
            result = result * i;
        }
        return result;
    }

    void NodeCluster::swap_mapping(std::size_t a, std::size_t b) {
        std::swap(mapping_[a], mapping_[b]);
    }

    bool NodeCluster::next() {

        if (indices_.size() == 1) {
            return true;
        }

        while (iterator_ < indices_.size()) {
            if (indices_[iterator_] < iterator_) {
                swap_mapping((iterator_ % 2 == 0 ?  0 : indices_[iterator_]), iterator_);
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

    int NodeCluster::permutations() const {
        return permutations_;
    }

    void NodeClassifier::addSubject(const std::string &label, const std::string &object) {
        if (categories.count(label)) {
            categories[label].addSubject(object);
        } else {
            NodeCategory nodeCategory;
            nodeCategory.addSubject(object);
            categories.insert(std::make_pair(label, nodeCategory));
        }
    }

    void NodeClassifier::addSubject(const std::string &label) {
        if (categories.count(label)) {
            categories[label].addSubject();
        } else {
            NodeCategory nodeCategory;
            nodeCategory.addSubject();
            categories.insert(std::make_pair(label, nodeCategory));
        }
    }

    void NodeClassifier::addObject(const std::string &label, const std::string &subject) {
        if (categories.count(label)) {
            categories[label].addObject(subject);
        } else {
            NodeCategory nodeCategory;
            nodeCategory.addObject(subject);
            categories.insert(std::make_pair(label, nodeCategory));
        }
    }

    void NodeClassifier::addObject(const std::string &label) {
        if (categories.count(label)) {
            categories[label].addObject();
        } else {
            NodeCategory nodeCategory;
            nodeCategory.addObject();
            categories.insert(std::make_pair(label, nodeCategory));
        }
    }

    void NodeClassifier::addGraph(const std::string &label) {
        if (categories.count(label)) {
            categories[label].addGraph();
        } else {
            NodeCategory nodeCategory;
            nodeCategory.addGraph();
            categories.insert(std::make_pair(label, nodeCategory));
        }
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

    NodeMapper::NodeMapper(int permutations, std::map<std::string, NodeCluster> mapping,
                           std::vector<NodeCluster> clusters)
            : mapping_(mapping), clusters_(clusters), permutations_(permutations), iterator_(0)
    { }

    std::unique_ptr<std::map<std::string, NodeCluster>>
    NodeMapper::merge(const std::map<NodeCategory, std::vector<std::string>> &reducedSource,
                      const std::map<NodeCategory, std::vector<std::string>> &reducedTarget,
                      std::vector<NodeCluster> &clusters) {

        if (reducedSource.size() != reducedTarget.size()) {
            return nullptr;
        }

        std::unique_ptr<std::map<std::string, NodeCluster>> mapping(new std::map<std::string, NodeCluster>());

        for (const auto &element : reducedSource) {

            if(reducedTarget.count(element.first) == 0)
                return nullptr;

            const std::vector<std::string>& target = reducedTarget.at(element.first);

            if (target.size() != element.second.size()) {
                return nullptr;
            }

            NodeCluster cluster = NodeCluster::create(element.second, target);
            clusters.push_back(cluster);
            for(const auto &l : element.second)
                mapping->insert(std::make_pair(l, cluster));
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
            for (NodeCluster cluster : clusters_) {
                if (!cluster.next()) {
                    break;
                }
            }
        }

        std::map<std::string, std::string> result;

        for (const auto &element : mapping_) {
            result.insert(std::make_pair(element.first, element.second.mapping(element.first)));
        }

        iterator_++;

        return result;
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

        std::vector<NodeCluster> clusters;

        auto mapping = merge(reducedSource, reducedTarget, clusters);
        if(!mapping)
            return {};

        int permutations = 1;
        std::accumulate(clusters.begin(), clusters.end(), permutations,
                        [](int a, const NodeCluster& b) { return a * b.permutations(); });

        return {permutations, *mapping, clusters};
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

            if (iteration >=  5000000) {
                std::cerr << "Too many permutations [" << mapper.permutations() << "]" << std::endl;
                return false;
            }
        }

        return false;
    }

}
