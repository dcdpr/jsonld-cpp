#ifndef LIBJSONLD_CPP_RDFDATASETUTILS_H
#define LIBJSONLD_CPP_RDFDATASETUTILS_H

#include "RDFDataset.h"

namespace RDFDatasetUtils {
    std::string toNQuads(const RDF::RDFDataset& dataset);

    std::string toNQuad(const RDF::Quad& triple, std::string *graphName);

    std::string toNQuad(const RDF::Quad& triple, std::string *graphName, std::string *bnode);

    void escape(const std::string& str, std::stringstream & ss);
    void unescape(const std::string& str, std::stringstream & ss);
    std::string unescape(const std::string& str);

    bool isHighSurrogate(char c);

    RDF::RDFDataset parseNQuads(std::string input);
}

#endif //LIBJSONLD_CPP_RDFDATASETUTILS_H
