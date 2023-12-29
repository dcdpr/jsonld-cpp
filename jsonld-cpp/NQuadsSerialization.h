#ifndef LIBJSONLD_CPP_NQUADSSERIALIZATION_H
#define LIBJSONLD_CPP_NQUADSSERIALIZATION_H

#include <string>
#include <sstream>

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
    std::string toNQuads(const RDF::RDFDataset &dataset);

    /**
     * Converts an RDFQuad into a string in the form of an N-Quad
     */
    std::string toNQuad(const RDF::RDFQuad &quad);

    /**
     * Converts an RDFTriple into a string in the form of an N-Quad
     */
    std::string toNQuad(const RDF::RDFTriple &triple);

    /**
     * Writes IRI value to given stringstream, escaping characters as necessary
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    void outputIRI(const std::string &value, std::stringstream &ss);

    /**
     * Returns escaped input string
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    std::string escape(const std::string &input);

    /**
     * Returns Un-escaped input string
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    std::string unescape(const std::string &input);
}

#endif //LIBJSONLD_CPP_NQUADSSERIALIZATION_H
