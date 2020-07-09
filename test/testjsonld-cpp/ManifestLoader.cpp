#include "ManifestLoader.h"
#include "RemoteDocument.h"

using json = nlohmann::json;

ManifestLoader::ManifestLoader(const std::string &manifestBase, const std::string &manifestName)
        : manifestBase(manifestBase), manifestName(manifestName) {}

std::map<std::string, TestCase> ManifestLoader::load() {

    std::string manifestPath = manifestBase + manifestName;
    RemoteDocument d = documentLoader.loadDocument(manifestPath);
    json manifest = d.getDocument();

    std::string baseUri = manifest["baseIri"];

    std::map<std::string, TestCase> result;

    for (auto& element : manifest["sequence"]) {
        TestCase tc = TestCase::create(element, manifestName, manifestBase, baseUri);
        result[tc.id] = tc;
    }

    return result;
}


