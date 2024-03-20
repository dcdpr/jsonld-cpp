#ifndef LIBJSONLD_CPP_RDFDATASET_H
#define LIBJSONLD_CPP_RDFDATASET_H

#include <vector>
#include <map>
#include <string>
#include <ostream>

#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/RDFTriple.h"

class JsonLdOptions;

namespace RDF {

    class RDFQuad;

    class RDFGraph {
    private:
        std::vector<RDFTriple> triples;
    public:
        typedef std::vector<RDFTriple>::size_type size_type;
        typedef std::vector<RDFTriple>::iterator iterator;
        typedef std::vector<RDFTriple>::const_iterator const_iterator;
        typedef std::vector<RDFTriple>::reference reference;

        void add(const RDFTriple& triple);

        std::string toString() const;

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        reference operator[]( size_type pos );

        bool empty() const noexcept;
        size_type size() const noexcept;
    };

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
        GraphNameToGraph storedGraphs;

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

    std::ostream &operator<<(std::ostream &os, const RDFDataset &rdfDataset);

}

#endif //LIBJSONLD_CPP_RDFDATASET_H
