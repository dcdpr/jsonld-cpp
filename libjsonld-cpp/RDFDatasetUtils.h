#ifndef LIBJSONLD_CPP_RDFDATASETUTILS_H
#define LIBJSONLD_CPP_RDFDATASETUTILS_H

#include "RDFDataset.h"

namespace RDFDatasetUtils {
    std::string toNQuads(const RDF::RDFDataset& dataset);

    std::string toNQuad(const RDF::Quad& triple, std::string *graphName);

    std::string toNQuad(const RDF::Quad& triple, std::string *graphName, std::string *bnode);

    void escape(const std::string& str, std::stringstream & ss);

    bool isHighSurrogate(char c);
}

#endif //LIBJSONLD_CPP_RDFDATASETUTILS_H
