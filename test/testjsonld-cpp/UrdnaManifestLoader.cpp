#include "UrdnaManifestLoader.h"
#include "RemoteDocument.h"

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
        element["@id"] = element["id"];
        element["@type"] = element["type"];
        element["input"] = element["action"];
        element["expect"] = element["result"];

        TestCase tc = TestCase::create(element, manifestName, manifestBase, baseUri);
        result.insert({tc.id, tc});
    }

    return result;
}


