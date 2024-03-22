#include "jsonld-cpp/RDFDataset.h"

#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/detail/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/NQuadsSerialization.h"


namespace RDF {

    RDFDataset::RDFDataset(const JsonLdOptions & options)
            : options(options) {
    }

    RDFGraph RDFDataset::getGraph(const std::string & graphName) const {
        if(storedGraphs.count(graphName))
            return storedGraphs.at(graphName);
        else
            return {};
    }

    void RDFDataset::setGraph(const RDFGraph & graph, const std::string & graphName) {
        storedGraphs[graphName] = graph;
    }

    void RDFDataset::addTripleToGraph(const std::string & graphName, const RDFTriple& triple) {
            storedGraphs[graphName].add(triple);
    }

    std::vector<RDFQuad> RDFDataset::getAllGraphsAsQuads() const {
        std::vector<RDFQuad> quads;
        for (const auto& g : storedGraphs) {
            std::string graphName = g.first;
            for(const auto &triple : g.second)
                quads.emplace_back(triple.getSubject(), triple.getPredicate(), triple.getObject(), &graphName);
        }
        return quads;
    }

    std::size_t RDFDataset::numGraphs() const {
        return storedGraphs.size();
    }

    bool RDFDataset::empty() const {
        return storedGraphs.empty();
    }

    RDFGraph::size_type RDFDataset::numTriples() const {
        RDFGraph::size_type count=0;
        for (const auto& graph : storedGraphs)
            count += graph.second.size();
        return count;
    }

    bool RDFGraph::empty() const noexcept {
        return triples.empty();
    }

    RDFGraph::size_type RDFGraph::size() const noexcept {
        return triples.size();
    }

    void RDFGraph::add(const RDFTriple& triple) {
        if(std::find(triples.begin(), triples.end(), triple) == triples.end()) {
            triples.push_back(triple);
        }
    }

    RDFGraph::iterator RDFGraph::begin() noexcept {
        return triples.begin();
    }

    RDFGraph::const_iterator RDFGraph::begin() const noexcept {
        return triples.begin();
    }

    RDFGraph::iterator RDFGraph::end() noexcept {
        return triples.end();
    }

    RDFGraph::const_iterator RDFGraph::end() const noexcept {
        return triples.end();
    }

    std::string RDFGraph::toString() const {
        std::string ret;
        for(const auto& i : triples)
            ret += NQuadsSerialization::toNQuad(i);
        return ret;
    }

    RDFTriple &RDFGraph::operator[](RDFGraph::size_type pos) {
        return triples[pos];
    }

    std::ostream &operator<<(std::ostream &os, const RDFDataset &rdfDataset) {
        os << NQuadsSerialization::toNQuads(rdfDataset);
        return os;
    }

}
