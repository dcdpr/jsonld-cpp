#ifndef LIBJSONLD_CPP_RDFDATASETUTILS_H
#define LIBJSONLD_CPP_RDFDATASETUTILS_H

#include <string>
#include <vector>

namespace RDF {
    class RDFDataset;
    class RDFTriple;
    class RDFQuad;
}

namespace RDFDatasetUtils {
    std::string toNQuads(const RDF::RDFDataset& dataset);

    std::string toNQuad(const RDF::RDFQuad& quad);
    std::string toNQuad(const RDF::RDFTriple& triple);

    std::string toNQuadForNormalization(const RDF::RDFQuad& quad, std::string *bnode);

    void escape(const std::string& str, std::stringstream & ss);
    void unescape(const std::string& str, std::stringstream & ss);
    std::string escape(const std::string& str);
    std::string unescape(const std::string& str);

    RDF::RDFDataset parseNQuads(std::string input);

    std::vector<std::string> splitLines(std::string &input);
}

#endif //LIBJSONLD_CPP_RDFDATASETUTILS_H
