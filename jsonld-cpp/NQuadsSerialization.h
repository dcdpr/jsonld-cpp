#ifndef LIBJSONLD_CPP_NQUADSSERIALIZATION_H
#define LIBJSONLD_CPP_NQUADSSERIALIZATION_H

#include <string>

namespace RDF {
    class RDFDataset;
    class RDFTriple;
    class RDFQuad;
}

/**
 * Functions to convert back and forth between RDF objects and strings containing N-Quads
 *
 *  N-Quads is a line-based, plain text format for encoding an RDF dataset.
 *
 *  See: https://www.w3.org/TR/n-quads/
 */
namespace NQuadsSerialization {

    /**
     * Parses a string containing RDF data in the form of N-Quads into an RDFDataset.
     */
    RDF::RDFDataset parse(std::string input);

    /**
     * Converts an RDFDataset into a string in the form of N-Quads
     */
    std::string toNQuads(const RDF::RDFDataset& dataset);

    /**
     * Converts an RDFQuad into a string in the form of an N-Quad
     */
    std::string toNQuad(const RDF::RDFQuad& quad);

    /**
     * Converts an RDFQuad into a string in the form of an N-Quad
     *
     * This function is slightly modified from toNQuad() in order to conform to the
     * RDF Canonicalization spec. See the description of the Hash First Degree Quads algorithm:
     * https://w3c-ccg.github.io/rdf-dataset-canonicalization/spec/#hash-first-degree-quads
     */
    std::string toNQuadForNormalization(const RDF::RDFQuad& quad, std::string *bnode);

    /**
     * Converts an RDFTriple into a string in the form of an N-Quad
     */
    std::string toNQuad(const RDF::RDFTriple& triple);

}

#endif //LIBJSONLD_CPP_NQUADSSERIALIZATION_H
