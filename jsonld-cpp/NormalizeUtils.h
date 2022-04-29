#ifndef LIBJSONLD_CPP_NORMALIZEUTILS_H
#define LIBJSONLD_CPP_NORMALIZEUTILS_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/UniqueNamer.h"
#include "jsonld-cpp/RDFDataset.h"

class NormalizeUtils {
private:
    std::vector<RDF::Quad> quads;
    std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> bnodes;
    std::map<std::string, std::string> cachedHashes;
    UniqueNamer uniqueNamer;
    JsonLdOptions opts;

    struct HashResult {
        std::string hash;
        UniqueNamer pathNamer;
    };

    HashResult hashPaths(const std::string& id, UniqueNamer pathUniqueNamer);

    std::string hashQuads(std::string id);

public:

    NormalizeUtils(
            std::vector<RDF::Quad> quads,
            std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> bnodes,
            UniqueNamer  iuniqueNamer,
            JsonLdOptions opts);

    std::string hashBlankNodes(const std::vector<std::string> &unnamed_);

    static std::shared_ptr<std::string> getAdjacentBlankNodeName(std::shared_ptr<RDF::Node> node, std::string id);

};

#endif //LIBJSONLD_CPP_NORMALIZEUTILS_H
