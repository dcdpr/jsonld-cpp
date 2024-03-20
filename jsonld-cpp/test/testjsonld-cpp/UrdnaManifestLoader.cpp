#include "UrdnaManifestLoader.h"

#include <jsonld-cpp/RemoteDocument.h>

using json = nlohmann::json;

UrdnaManifestLoader::UrdnaManifestLoader(const std::string &imanifestBase, const std::string &imanifestName)
        : manifestBase(imanifestBase), manifestName(imanifestName) {}

std::map<std::string, TestCase> UrdnaManifestLoader::load() {

    std::string manifestPath = manifestBase + manifestName;
    auto d = documentLoader.loadDocument(manifestPath);
    json manifest = d->getJSONContent();

    std::string baseUri = "https://www.example.com/";

    std::map<std::string, TestCase> result;

    for (auto& element : manifest["entries"]) {
        // make copies of existing data with labels expected by TestCase
        if(element.contains("id"))
            element["@id"] = element["id"];

        if(element.contains("type"))
            element["@type"] = element["type"];

        if(element.contains("action"))
            element["input"] = element["action"];

        if(element.contains("result"))
            element["expect"] = element["result"];

        if(element.contains("hashAlgorithm"))
            element["hashAlgorithm"] = element["hashAlgorithm"];

        TestCase tc = TestCase::create(element, manifestName, manifestBase, baseUri);
        result.insert({tc.id, tc});
    }

    return result;
}


