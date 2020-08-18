#include <FileLoader.h>
#include "TestCase.h"
#include "JsonLdConsts.h"
#include "UriBaseRewriter.h"
#include "TestCaseOptions.h"

TestCase::TestCase(const std::string &itestsBase) : testsBase(itestsBase) {}

TestCase TestCase::create(
        nlohmann::json o,
        const std::string& manifestUri,
        const std::string& manifestBase,
        const std::string& baseUri) {

    TestCase testCase(manifestBase);

    testCase.id = o[JsonLdConsts::ID];

    testCase.uri = baseUri + manifestUri.substr(0, manifestUri.size() - strlen(".jsonld")) + testCase.id;

    for(auto& element : o[JsonLdConsts::TYPE]) {
        testCase.type.insert(element.get<std::string>());
    }

    testCase.name = o["name"];

    testCase.input = o.contains("input") ?
                     baseUri + o["input"].get<std::string>() :
                     "";

    testCase.context = o.contains("context") ?
                       baseUri + o["context"].get<std::string>() :
                       "";

    testCase.expect = o.contains("expect") ?
                       baseUri + o["expect"].get<std::string>() :
                       "";

    testCase.frame = o.contains("frame") ?
                       baseUri + o["frame"].get<std::string>() :
                       "";

    // todo: not needed until we do the NegativeEvaluationTests
    // testCase.expectErrorCode = ...

    if(o.contains("option")) {
        testCase.options = TestCaseOptions::create(o["option"], baseUri);
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
    //jsonLdOptions.setOrdered(true);

    options.copyTo(jsonLdOptions);

    return jsonLdOptions;

}

