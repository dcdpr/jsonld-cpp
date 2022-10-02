#ifndef LIBJSONLD_CPP_RDFDATASETCOMPARISON_H
#define LIBJSONLD_CPP_RDFDATASETCOMPARISON_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/RDFNode.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/RDFDataset.h"
#include <memory>
#include <string>
#include <iostream>
#include <set>
#include <utility>

namespace RDF {

    class NodeCategory {
    private:
        int objectCount = 0;
        int subjectCount = 0;
        int graphCount = 0;

        std::set<std::string> objects;
        std::set<std::string> subjects;

    public:
        void addObject(const std::string & object) {
            objectCount++;
            objects.insert(object);
        }

        void addObject() {
            objectCount++;
        }

        void addSubject(const std::string & subject) {
            subjectCount++;
            objects.insert(subject);
        }

        void addSubject() {
            subjectCount++;
        }

        void addGraph() {
            graphCount++;
        }

        bool operator<(const NodeCategory &rhs) const {
            return std::tie(objectCount, subjectCount, graphCount, objects, subjects) <
                   std::tie(rhs.objectCount, rhs.subjectCount, rhs.graphCount, rhs.objects, rhs.subjects);
        }

        bool operator>(const NodeCategory &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const NodeCategory &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const NodeCategory &rhs) const {
            return !(*this < rhs);
        }

        bool operator==(const NodeCategory &rhs) const {
            return std::tie(objectCount, subjectCount, graphCount, objects, subjects) ==
                   std::tie(rhs.objectCount, rhs.subjectCount, rhs.graphCount, rhs.objects, rhs.subjects);
        }

        bool operator!=(const NodeCategory &rhs) const {
            return !(rhs == *this);
        }
    };

    class NodeCluster {
    private:
        std::map<std::string, std::size_t> source_; // todo: maybe make this size_t instead of int?

        std::vector<std::string> target_;
        std::vector<std::size_t> indices_;
        std::vector<std::size_t> mapping_;

        std::size_t iterator_;
        int permutations_;

    public:
        NodeCluster(
                std::map<std::string, std::size_t> source,
                std::vector<std::string> target,
                int permutations)
                : source_(std::move(source)), target_(std::move(target)), permutations_(permutations) {
            indices_.assign(source_.size(), 0);
            mapping_.assign(source_.size(), 0);
            iterator_ = 0;
            init();
        }

        void init() {
            indices_.assign(indices_.size(), 0);
            std::iota(mapping_.begin(), mapping_.end(), 0);
            iterator_ = 0;
        }

        static NodeCluster create(const std::vector<std::string>& source, const std::vector<std::string>& target) {

            if (source.size() != target.size()) {
                throw std::invalid_argument("source.size() != target.size() in NodeCluster");
            }

            std::map<std::string, std::size_t> sourceMap;
            for (const auto& s : source) {
                sourceMap.insert(std::make_pair(s, sourceMap.size()));
            }

            return NodeCluster(sourceMap, target, factorial(static_cast<int>(source.size())));
        }

        static int factorial(int number) {
            int result = 1;
            for(int i=1; i <= number; i++){
                result = result * i;
            }
            return result;
        }

        void swap_mapping(std::size_t a, std::size_t b) {
            std::swap(mapping_[a], mapping_[b]);
        }

        bool next() {

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

        std::string mapping(const std::string& label) const {
            return target_[mapping_[source_.at(label)]];
        }

        int permutations() const {
            return permutations_;
        }

    };

    class NodeClassifier {
    private:
        std::map<std::string, NodeCategory> categories;

        void addSubject(const std::string &label, const std::string &object) {
            if (categories.count(label)) {
                categories[label].addSubject(object);
            } else {
                NodeCategory nodeCategory;
                nodeCategory.addSubject(object);
                categories.insert(std::make_pair(label, nodeCategory));
            }
        }

        void addSubject(const std::string &label) {
            if (categories.count(label)) {
                categories[label].addSubject();
            } else {
                NodeCategory nodeCategory;
                nodeCategory.addSubject();
                categories.insert(std::make_pair(label, nodeCategory));
            }
        }

        void addObject(const std::string &label, const std::string &subject) {
            if (categories.count(label)) {
                categories[label].addObject(subject);
            } else {
                NodeCategory nodeCategory;
                nodeCategory.addObject(subject);
                categories.insert(std::make_pair(label, nodeCategory));
            }
        }

        void addObject(const std::string &label) {
            if (categories.count(label)) {
                categories[label].addObject();
            } else {
                NodeCategory nodeCategory;
                nodeCategory.addObject();
                categories.insert(std::make_pair(label, nodeCategory));
            }
        }

        void addGraph(const std::string &label) {
            if (categories.count(label)) {
                categories[label].addGraph();
            } else {
                NodeCategory nodeCategory;
                nodeCategory.addGraph();
                categories.insert(std::make_pair(label, nodeCategory));
            }
        }

    public:
        void add(const RDFQuad &nquad) {

            if (nquad.getSubject()->isBlankNode()) {
                if (nquad.getObject()->isIRI())
                    addSubject(nquad.getSubject()->getValue(), nquad.getObject()->getValue());
                else
                    addSubject(nquad.getSubject()->getValue());
            }
            if (nquad.getObject()->isBlankNode()) {
                // todo: should the adds() here be Object?
                if (nquad.getSubject()->isIRI())
                    addSubject(nquad.getObject()->getValue(), nquad.getSubject()->getValue());
                else
                    addSubject(nquad.getObject()->getValue());
            }
            if (nquad.getGraph() && nquad.getGraph()->isBlankNode()) {
                addGraph(nquad.getGraph()->getValue());
            }
        }

        using size_type = std::map<std::string, NodeCategory>::size_type;

        size_type size() {
            return categories.size();
        }

        std::map<NodeCategory, std::vector<std::string>> reduce() {

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
    };

    class NodeMapper {
    private:
        std::map<std::string, NodeCluster> mapping_;
        std::vector<NodeCluster> clusters_;
        int permutations_;
        int iterator_;

        NodeMapper()
                : permutations_(-1), iterator_(0)
        { }

        NodeMapper(int permutations, std::map<std::string, NodeCluster> mapping, std::vector<NodeCluster> clusters)
                : mapping_(mapping), clusters_(clusters), permutations_(permutations), iterator_(0)
        { }

        static std::unique_ptr<std::map<std::string, NodeCluster>> merge(
                const std::map<NodeCategory, std::vector<std::string>>& reducedSource,
                const std::map<NodeCategory, std::vector<std::string>>& reducedTarget,
                std::vector<NodeCluster> & clusters) {

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

    public:

        int permutations() const {
            return permutations_;
        }

        bool hasNext() const {
            return iterator_ != -1 && iterator_ < permutations_;
        }

        std::map<std::string, std::string> next() {

            //if (!hasNext() || clusters_ == null) {
            if (!hasNext()) {
                throw std::invalid_argument("!hasNext() in NodeMapper");
            }


            if (iterator_ > 0) {
                bool overflow = true;

                for (NodeCluster cluster : clusters_) {
                    if (overflow) {
                        overflow = cluster.next();
                    }
                    if (!overflow) {
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

        static NodeMapper create(const std::vector<RDFQuad>& from, const std::vector<RDFQuad>& to) {

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
                return NodeMapper();
            }

            auto reducedSource = sourceClassifier.reduce();

            auto reducedTarget = targetClassifier.reduce();

            std::vector<NodeCluster> clusters;

            auto mapping = merge(reducedSource, reducedTarget, clusters);
            if(!mapping)
                return NodeMapper();

            int permutations = 1;
            std::accumulate(clusters.begin(), clusters.end(), permutations,
                            [](int a, const NodeCluster& b) { return a * b.permutations(); });

            return NodeMapper(permutations, *mapping, clusters);
        }

    };

    bool areIsomorphic(const RDFDataset & dataset1, const RDFDataset & dataset2);
    bool compareGraphName(const std::shared_ptr<Node>& name1, const std::shared_ptr<Node>& name2, std::map<std::string, std::string> * mapping);
    bool compareObject(const std::shared_ptr<Node>& object1, const std::shared_ptr<Node>& object2, std::map<std::string, std::string> * mapping);
    bool comparePredicate(const std::shared_ptr<Node>& predicate1, const std::shared_ptr<Node>& predicate2);
    bool compareSubject(const std::shared_ptr<Node>& subject1, const std::shared_ptr<Node>& subject2, std::map<std::string, std::string> * mapping);
    bool compareNQuad(RDFQuad & nquad1, RDFQuad & nquad2, std::map<std::string, std::string> * mapping);
    bool compareNQuads(const std::vector<RDFQuad>& nquads1, const std::vector<RDFQuad>& nquads2, std::map<std::string, std::string> * mapping);

}

#endif //LIBJSONLD_CPP_RDFDATASETCOMPARISON_H
