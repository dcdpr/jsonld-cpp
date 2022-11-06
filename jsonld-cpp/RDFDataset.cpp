#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/JsonLdError.h"
#include "RDFRegex.h"
#include <regex>

using nlohmann::json;

namespace {
    std::vector<std::string> splitLines(std::string &input) {
        std::vector<std::string> lines;
        std::regex rgx(RDFRegex::EOLN);
        std::sregex_token_iterator i(input.begin(), input.end(), rgx,-1);
        std::sregex_token_iterator end;

        for ( ; i != end; ++i)
            lines.push_back(*i);
        return lines;
    }
}

namespace RDF {

    RDFDataset::RDFDataset(const JsonLdOptions & ioptions)
            : options(ioptions) {

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
        if(!storedGraphs.count(graphName) ||
           std::find(storedGraphs[graphName].cbegin(), storedGraphs[graphName].cend(), triple) == storedGraphs[graphName].cend())
            storedGraphs[graphName].push_back(triple);
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
        for (const auto& g : storedGraphs)
            count += g.second.size();
        return count;
    }

}
