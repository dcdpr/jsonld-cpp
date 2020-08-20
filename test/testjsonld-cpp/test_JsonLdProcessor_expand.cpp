#include "JsonLdProcessor.h"
#include "testHelpers.h"
#include "ManifestLoader.h"
#include <fstream>

using nlohmann::json;

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop


void performExpandTest(TestCase &testCase) {
    JsonLdOptions options = testCase.getOptions();

    std::cout << "Id: " << testCase.id << std::endl;
    std::cout << "Name: " << testCase.name << std::endl;
    if(!testCase.options.specVersion.empty())
        std::cout << "SpecVersion: " << testCase.options.specVersion;
    if(testCase.options.specVersion == "json-ld-1.1") {
        std::cout << " ...skipping for now." << std::endl;
        SUCCEED();
        return;
    }
    else
        std::cout << std::endl;

    json expanded;
    try {
        expanded = JsonLdProcessor::expand(testCase.input, options);
    }
    catch (JsonLdError &e) {
        std::cout << "JsonLdError: " << e.what() << std::endl;
        if(testCase.type.count("jld:NegativeEvaluationTest")) {
            std::string error = e.what();
            if(error.find(testCase.expectErrorCode) != std::string::npos) {
                SUCCEED();
                return;
            }
        }
    }
    catch (std::runtime_error &e) {
        FAIL() << e.what();
    }

    std::unique_ptr<RemoteDocument> expectedDocument =
            options.getDocumentLoader()->loadDocument(testCase.expect);

    const json& expected = expectedDocument->getJSONContent();

    EXPECT_TRUE(JsonLdUtils::deepCompare(expected, expanded));
    //std::cout << expected.dump() << std::endl;
    //std::cout << expanded.dump() << std::endl;
}

void performExpandTestFromManifest(const std::string& testName, const std::string& manifestName="expand-manifest.jsonld") {

    ManifestLoader manifestLoader(
            resolvePath("test/testjsonld-cpp/test_data/"),
            manifestName);
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    auto testCase = testCases.at(testName);

    performExpandTest(testCase);

}

TEST(JsonLdProcessorTest, expand_0001) {
    performExpandTestFromManifest("#t0001");
}

TEST(JsonLdProcessorTest, expand_0002) {
    performExpandTestFromManifest("#t0002");
}

TEST(JsonLdProcessorTest, expand_0003) {
    performExpandTestFromManifest("#t0003");
}

TEST(JsonLdProcessorTest, expand_0004) {
    performExpandTestFromManifest("#t0004");
}

TEST(JsonLdProcessorTest, expand_0005) {
    performExpandTestFromManifest("#t0005");
}

TEST(JsonLdProcessorTest, expand_0006) {
    performExpandTestFromManifest("#t0006");
}

TEST(JsonLdProcessorTest, expand_0007) {
    performExpandTestFromManifest("#t0007");
}

TEST(JsonLdProcessorTest, expand_0008) {
    performExpandTestFromManifest("#t0008");
}

TEST(JsonLdProcessorTest, expand_0009) {
    performExpandTestFromManifest("#t0009");
}

TEST(JsonLdProcessorTest, expand_0010) {
    performExpandTestFromManifest("#t0010");
}

TEST(JsonLdProcessorTest, expand_0011) {
    performExpandTestFromManifest("#t0011");
}

TEST(JsonLdProcessorTest, expand_0012) {
    performExpandTestFromManifest("#t0012");
}

TEST(JsonLdProcessorTest, expand_0013) {
    performExpandTestFromManifest("#t0013");
}

TEST(JsonLdProcessorTest, expand_0014) {
    performExpandTestFromManifest("#t0014");
}

TEST(JsonLdProcessorTest, expand_0015) {
    performExpandTestFromManifest("#t0015");
}

TEST(JsonLdProcessorTest, expand_0016) {
    performExpandTestFromManifest("#t0016");
}

TEST(JsonLdProcessorTest, expand_0017) {
    performExpandTestFromManifest("#t0017");
}

TEST(JsonLdProcessorTest, expand_0018) {
    performExpandTestFromManifest("#t0018");
}

TEST(JsonLdProcessorTest, expand_0019) {
    performExpandTestFromManifest("#t0019");
}

TEST(JsonLdProcessorTest, expand_0020) {
    performExpandTestFromManifest("#t0020");
}

TEST(JsonLdProcessorTest, expand_0021) {
    performExpandTestFromManifest("#t0021");
}

TEST(JsonLdProcessorTest, expand_0022) {
    performExpandTestFromManifest("#t0022");
}

TEST(JsonLdProcessorTest, expand_0023) {
    performExpandTestFromManifest("#t0023");
}

TEST(JsonLdProcessorTest, expand_0024) {
    performExpandTestFromManifest("#t0024");
}

TEST(JsonLdProcessorTest, expand_0025) {
    performExpandTestFromManifest("#t0025");
}

TEST(JsonLdProcessorTest, expand_0026) {
    performExpandTestFromManifest("#t0026");
}

TEST(JsonLdProcessorTest, expand_0027) {
    performExpandTestFromManifest("#t0027");
}

TEST(JsonLdProcessorTest, expand_0028) {
    performExpandTestFromManifest("#t0028");
}

TEST(JsonLdProcessorTest, expand_0029) {
    performExpandTestFromManifest("#t0029");
}

TEST(JsonLdProcessorTest, expand_0030) {
    performExpandTestFromManifest("#t0030");
}

TEST(JsonLdProcessorTest, expand_0031) {
    performExpandTestFromManifest("#t0031");
}

TEST(JsonLdProcessorTest, expand_0032) {
    performExpandTestFromManifest("#t0032");
}

TEST(JsonLdProcessorTest, expand_0033) {
    performExpandTestFromManifest("#t0033");
}

TEST(JsonLdProcessorTest, expand_0034) {
    performExpandTestFromManifest("#t0034");
}

TEST(JsonLdProcessorTest, expand_0035) {
    performExpandTestFromManifest("#t0035");
}

TEST(JsonLdProcessorTest, expand_0036) {
    performExpandTestFromManifest("#t0036");
}

TEST(JsonLdProcessorTest, expand_0037) {
    performExpandTestFromManifest("#t0037");
}

TEST(JsonLdProcessorTest, expand_0038) {
    performExpandTestFromManifest("#t0038");
}

TEST(JsonLdProcessorTest, expand_0039) {
    performExpandTestFromManifest("#t0039");
}

TEST(JsonLdProcessorTest, expand_0040) {
    performExpandTestFromManifest("#t0040");
}

TEST(JsonLdProcessorTest, expand_0041) {
    performExpandTestFromManifest("#t0041");
}

TEST(JsonLdProcessorTest, expand_0042) {
    performExpandTestFromManifest("#t0042");
}

TEST(JsonLdProcessorTest, expand_0043) {
    performExpandTestFromManifest("#t0043");
}

TEST(JsonLdProcessorTest, expand_0044) {
    performExpandTestFromManifest("#t0044");
}

TEST(JsonLdProcessorTest, expand_0045) {
    performExpandTestFromManifest("#t0045");
}

TEST(JsonLdProcessorTest, expand_0046) {
    performExpandTestFromManifest("#t0046");
}

TEST(JsonLdProcessorTest, expand_0047) {
    performExpandTestFromManifest("#t0047");
}

TEST(JsonLdProcessorTest, expand_0048) {
    performExpandTestFromManifest("#t0048");
}

TEST(JsonLdProcessorTest, expand_0049) {
    performExpandTestFromManifest("#t0049");
}

TEST(JsonLdProcessorTest, expand_0050) {
    performExpandTestFromManifest("#t0050");
}

TEST(JsonLdProcessorTest, expand_0051) {
    performExpandTestFromManifest("#t0051");
}

TEST(JsonLdProcessorTest, expand_0052) {
    performExpandTestFromManifest("#t0052");
}

TEST(JsonLdProcessorTest, expand_0053) {
    performExpandTestFromManifest("#t0053");
}

TEST(JsonLdProcessorTest, expand_0054) {
    performExpandTestFromManifest("#t0054");
}

TEST(JsonLdProcessorTest, expand_0055) {
    performExpandTestFromManifest("#t0055");
}

TEST(JsonLdProcessorTest, expand_0056) {
    performExpandTestFromManifest("#t0056");
}

TEST(JsonLdProcessorTest, expand_0057) {
    performExpandTestFromManifest("#t0057");
}

TEST(JsonLdProcessorTest, expand_0058) {
    performExpandTestFromManifest("#t0058");
}

TEST(JsonLdProcessorTest, expand_0059) {
    performExpandTestFromManifest("#t0059");
}

TEST(JsonLdProcessorTest, expand_0060) {
    performExpandTestFromManifest("#t0060");
}

TEST(JsonLdProcessorTest, expand_0061) {
    performExpandTestFromManifest("#t0061");
}

TEST(JsonLdProcessorTest, expand_0062) {
    performExpandTestFromManifest("#t0062");
}

TEST(JsonLdProcessorTest, expand_0063) {
    performExpandTestFromManifest("#t0063");
}

TEST(JsonLdProcessorTest, expand_0064) {
    performExpandTestFromManifest("#t0064");
}

TEST(JsonLdProcessorTest, expand_0065) {
    performExpandTestFromManifest("#t0065");
}

TEST(JsonLdProcessorTest, expand_0066) {
    performExpandTestFromManifest("#t0066");
}

TEST(JsonLdProcessorTest, expand_0067) {
    performExpandTestFromManifest("#t0067");
}

TEST(JsonLdProcessorTest, expand_0068) {
    performExpandTestFromManifest("#t0068");
}

TEST(JsonLdProcessorTest, expand_0069) {
    performExpandTestFromManifest("#t0069");
}

TEST(JsonLdProcessorTest, expand_0070) {
    performExpandTestFromManifest("#t0070");
}

TEST(JsonLdProcessorTest, expand_0071) {
    performExpandTestFromManifest("#t0071");
}

TEST(JsonLdProcessorTest, expand_0072) {
    performExpandTestFromManifest("#t0072");
}

TEST(JsonLdProcessorTest, expand_0073) {
    performExpandTestFromManifest("#t0073");
}

TEST(JsonLdProcessorTest, expand_0074) {
    performExpandTestFromManifest("#t0074");
}

TEST(JsonLdProcessorTest, expand_0075) {
    performExpandTestFromManifest("#t0075");
}

TEST(JsonLdProcessorTest, expand_0076) {
    performExpandTestFromManifest("#t0076");
}

// Disabled this test because we need to implement expandContext option
//TEST(JsonLdProcessorTest, expand_0077) {
//    performExpandTestFromManifest("#t0077");
//}

TEST(JsonLdProcessorTest, expand_0078) {
    performExpandTestFromManifest("#t0078");
}

TEST(JsonLdProcessorTest, expand_0088) {
    performExpandTestFromManifest("#t0088");
}

TEST(JsonLdProcessorTest, expand_0114) {
    performExpandTestFromManifest("#t0114");
}

TEST(JsonLdProcessorTest, expand_0115) {
    performExpandTestFromManifest("#t0115");
}


TEST(JsonLdProcessorTest, expand_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performExpandTestFromManifest("#t0300", "expand-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, expand_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performExpandTestFromManifest("#t0301", "expand-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, expand_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performExpandTestFromManifest("#t0302", "expand-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, expand_with_manifest) {

    ManifestLoader manifestLoader(resolvePath("test/testjsonld-cpp/test_data/"), "expand-manifest.jsonld");
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    for(auto & testCaseEntry : testCases) {
        std::cout << testCaseEntry.first << std::endl;

        auto testCase = testCaseEntry.second;

        performExpandTest(testCase);
    }

}

