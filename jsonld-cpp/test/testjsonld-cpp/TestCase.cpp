#include "TestCase.h"
#include <jsonld-cpp/FileLoader.h>
#include <jsonld-cpp/JsonLdConsts.h>
#include "UriBaseRewriter.h"
#include "TestCaseOptions.h"

namespace {

    std::string getEntryOr(const nlohmann::json &manifestEntry, const std::string & entryKey, const std::string & defaultEntry) {
        return manifestEntry.contains(entryKey) ?
               manifestEntry[entryKey].get<std::string>() : defaultEntry;
    }

}

TestCase::TestCase(const std::string &itestsBase) : testsBase(itestsBase) {}

TestCase TestCase::create(
        nlohmann::json manifestEntry,
        const std::string& manifestUri,
        const std::string& manifestBase,
        const std::string& baseUri) {

    TestCase testCase(manifestBase);

    testCase.id = manifestEntry[JsonLdConsts::ID].get<std::string>();

    testCase.uri = baseUri + manifestUri.substr(0, manifestUri.size() - strlen(".jsonld")) + testCase.id;

    for(auto& element : manifestEntry[JsonLdConsts::TYPE]) {
        testCase.type.insert(element.get<std::string>());
    }

    testCase.name = manifestEntry["name"].get<std::string>();

    testCase.input = baseUri + getEntryOr(manifestEntry, "input", "");

    testCase.context = baseUri + getEntryOr(manifestEntry, "context", "");

    testCase.expect = baseUri + getEntryOr(manifestEntry, "expect", "");

    testCase.frame = baseUri + getEntryOr(manifestEntry, "frame", "");

    testCase.expectErrorCode = getEntryOr(manifestEntry, "expectErrorCode", "");

    testCase.hashAlgorithm = HashAlgorithm::sha256;
    if(manifestEntry.contains("hashAlgorithm")) {
        if(manifestEntry["hashAlgorithm"].get<std::string>() == "SHA384")
            testCase.hashAlgorithm = HashAlgorithm::sha384;
    }

    if(manifestEntry.contains("option")) {
        testCase.options = TestCaseOptions::create(manifestEntry["option"], baseUri);
    }

    testCase.baseUri = baseUri;

    return testCase;
}

JsonLdOptions TestCase::getOptions() {
    std::unique_ptr<DocumentLoader> loader =
            std::unique_ptr<DocumentLoader>(new UriBaseRewriter(
                    baseUri,
                    testsBase,
                    std::unique_ptr<FileLoader>(new FileLoader())
            ));

    JsonLdOptions jsonLdOptions(std::move(loader));

    if(hashAlgorithm == HashAlgorithm::sha256)
        jsonLdOptions.setHashAlgorithm("SHA256");
    else if(hashAlgorithm == HashAlgorithm::sha384)
        jsonLdOptions.setHashAlgorithm("SHA384");

    options.copyTo(jsonLdOptions);

    return jsonLdOptions;

}

