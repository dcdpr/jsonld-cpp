#ifndef LIBJSONLD_CPP_NORMALIZEUTILS_H
#define LIBJSONLD_CPP_NORMALIZEUTILS_H


#include <json.hpp>
#include "JsonLdOptions.h"
#include "UniqueIdentifierGenerator.h"
#include "RDFDataset.h"

class NormalizeUtils {
private:
    std::vector<RDF::Quad> quads;
    std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> bnodes;
    std::map<std::string, std::string> cachedHashes;
    UniqueIdentifierGenerator uniqueIdentifierGenerator;
    JsonLdOptions opts;

    struct HashResult {
        std::string hash;
        UniqueIdentifierGenerator pathNamer;
    };

    HashResult hashPaths(std::string id, UniqueIdentifierGenerator pathNameGenerator);

    std::string hashQuads(std::string id);

public:

    NormalizeUtils(
            const std::vector<RDF::Quad> &quads,
            const std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> &bnodes,
            const UniqueIdentifierGenerator & uniqueIdentifierGenerator,
            const JsonLdOptions &opts);

    std::string hashBlankNodes(const std::vector<std::string> &unnamed_);

    static std::shared_ptr<std::string> getAdjacentBlankNodeName(std::shared_ptr<RDF::Node> node, std::string id);

};


#endif //LIBJSONLD_CPP_NORMALIZEUTILS_H
