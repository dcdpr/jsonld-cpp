#ifndef LIBJSONLD_CPP_RDFDATASET_H
#define LIBJSONLD_CPP_RDFDATASET_H

#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include <vector>
#include <map>
#include <string>


class JsonLdOptions;

namespace RDF {

    typedef std::vector<RDFTriple> RDFGraph;

    /**
     * An RDFDataset is a collection of RDFGraphs, and comprises:
     *
     * * Exactly one default graph, being an RDFGraph. The default graph does not have a name
     *   and MAY be empty.
     * * Zero or more named graphs. Each named graph is a pair consisting of graphName, and an
     *   RDFGraph. graphNames are unique within an RDF dataset.
     */
    class RDFDataset {
    private:
        typedef std::map<std::string, RDFGraph> GraphNameToGraph;
        JsonLdOptions options;
        GraphNameToGraph storedGraphs; // todo: not a good name

    public:
        explicit RDFDataset(const JsonLdOptions & options);

        /**
         * Returns a copy of the stored RDFGraph at graphName. If it doesn't exist, an empty
         * RDFGraph is returned.
         */
        RDFGraph getGraph(const std::string & graphName) const;

        /**
         * Save RDFGraph graph at graphName
         */
        void setGraph(const RDFGraph & graph, const std::string & graphName);

        /**
         * Add RDFTriple triple to RDFGraph at graphName if RDFTriple does not already exist
         */
        void addTripleToGraph(const std::string & graphName, const RDFTriple& triple);

        /**
         * Return collection of all stored RDFTriples in the RDFGraphs converted to RDFQuads
         */
        std::vector<RDFQuad> getAllGraphsAsQuads() const;

        /**
         * Return the number of stored RDFGraphs
         */
        std::size_t numGraphs() const;

        /**
         * Return if the RDFDataset is empty (zero stored RDFGraphs)
         */
        bool empty() const;

        /**
         * Return the number of stored RDFTriples in all the stored RDFGraphs
         */
        RDFGraph::size_type numTriples() const;
    };

}

#endif //LIBJSONLD_CPP_RDFDATASET_H
