#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/JsonLdError.h>
#include <jsonld-cpp/JsonLdProcessor.h>
#include <jsonld-cpp/FileLoader.h>
#include <jsonld-cpp/RemoteDocument.h>
#include <jsonld-cpp/NQuadsSerialization.h>
#include "UrdnaManifestLoader.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

using nlohmann::json;

// test suite fixture class
class JsonLdProcessorNormalizeRdfc10Test : public ::testing::Test {
public:
    static void SetUpTestCase() {
        std::string resource_dir = TEST_RESOURCE_DIR;
        UrdnaManifestLoader manifestLoader(
                resource_dir + "test_data/",
                mainManifestName);
        testCases = manifestLoader.load();
    }

    // resources shared by all tests.
    static std::string mainManifestName;
    static std::map<std::string, TestCase> testCases;

    static void performNormalizeTest(TestCase &testCase) {
        JsonLdOptions options = testCase.getOptions();

        std::cout << "Id: " << testCase.id << std::endl;
        std::cout << "Name: " << testCase.name << std::endl;
        if(!testCase.options.specVersion.empty())
            std::cout << "SpecVersion: " << testCase.options.specVersion << std::endl;

        std::string normalized;
        try {
            normalized = JsonLdProcessor::normalize(testCase.input, options);
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
            FAIL() << e.what();
        }
        catch (std::runtime_error &e) {
            FAIL() << e.what();
        }

        std::unique_ptr<RemoteDocument> expectedDocument =
                options.getDocumentLoader()->loadDocument(testCase.expect);

        const std::string & expected =
                NQuadsSerialization::toNQuads(expectedDocument->getRDFContent());

        EXPECT_EQ(normalized, expected);

    }

    static void performNormalizeTestFromAlternateManifest(const std::string& testName, const std::string& manifestName) {

        std::string resource_dir = TEST_RESOURCE_DIR;
        UrdnaManifestLoader manifestLoader(
                resource_dir + "test_data/",
                manifestName);
        std::map<std::string, TestCase> localTestCases = manifestLoader.load();

        auto testCase = localTestCases.at(testName);

        performNormalizeTest(testCase);
    }

    void performNormalizeTestFromManifest(const std::string& testName, const std::string& manifestName="") {

        if(!manifestName.empty())
            return performNormalizeTestFromAlternateManifest(testName, manifestName);

        auto testCase = testCases.at(testName);

        performNormalizeTest(testCase);
    }

};

std::string JsonLdProcessorNormalizeRdfc10Test::mainManifestName = "rdfc10-manifest.jsonld";
std::map<std::string, TestCase> JsonLdProcessorNormalizeRdfc10Test::testCases;


TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test001) {
    performNormalizeTestFromManifest("#test001c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test002) {
    performNormalizeTestFromManifest("#test002c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test003) {
    performNormalizeTestFromManifest("#test003c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test004) {
    performNormalizeTestFromManifest("#test004c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test005) {
    performNormalizeTestFromManifest("#test005c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test006) {
    performNormalizeTestFromManifest("#test006c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test007) {
    performNormalizeTestFromManifest("#test007c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test008) {
    performNormalizeTestFromManifest("#test008c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test009) {
    performNormalizeTestFromManifest("#test009c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test010) {
    performNormalizeTestFromManifest("#test010c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test011) {
    performNormalizeTestFromManifest("#test011c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test012) {
    performNormalizeTestFromManifest("#test012c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test013) {
    performNormalizeTestFromManifest("#test013c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test014) {
    performNormalizeTestFromManifest("#test014c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test015) {
    performNormalizeTestFromManifest("#test015c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test016) {
    performNormalizeTestFromManifest("#test016c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test017) {
    performNormalizeTestFromManifest("#test017c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test018) {
    performNormalizeTestFromManifest("#test018c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test019) {
    performNormalizeTestFromManifest("#test019c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test020) {
    performNormalizeTestFromManifest("#test020c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test021) {
    performNormalizeTestFromManifest("#test021c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test022) {
    performNormalizeTestFromManifest("#test022c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test023) {
    performNormalizeTestFromManifest("#test023c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test024) {
    performNormalizeTestFromManifest("#test024c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test025) {
    performNormalizeTestFromManifest("#test025c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test026) {
    performNormalizeTestFromManifest("#test026c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test027) {
    performNormalizeTestFromManifest("#test027c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test028) {
    performNormalizeTestFromManifest("#test028c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test029) {
    performNormalizeTestFromManifest("#test029c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test030) {
    performNormalizeTestFromManifest("#test030c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test031) {
    performNormalizeTestFromManifest("#test031c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test032) {
    performNormalizeTestFromManifest("#test032c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test033) {
    performNormalizeTestFromManifest("#test033c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test034) {
    performNormalizeTestFromManifest("#test034c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test035) {
    performNormalizeTestFromManifest("#test035c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test036) {
    performNormalizeTestFromManifest("#test036c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test037) {
    performNormalizeTestFromManifest("#test037c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test038) {
    performNormalizeTestFromManifest("#test038c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test039) {
    performNormalizeTestFromManifest("#test039c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test040) {
    performNormalizeTestFromManifest("#test040c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test041) {
    performNormalizeTestFromManifest("#test041c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test042) {
    performNormalizeTestFromManifest("#test042c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test043) {
    performNormalizeTestFromManifest("#test043c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test044) {
    performNormalizeTestFromManifest("#test044c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test045) {
    performNormalizeTestFromManifest("#test045c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test046) {
    performNormalizeTestFromManifest("#test046c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test047) {
    performNormalizeTestFromManifest("#test047c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test048) {
    performNormalizeTestFromManifest("#test048c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test049) {
    performNormalizeTestFromManifest("#test049c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test050) {
    performNormalizeTestFromManifest("#test050c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test051) {
    performNormalizeTestFromManifest("#test051c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test052) {
    performNormalizeTestFromManifest("#test052c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test053) {
    performNormalizeTestFromManifest("#test053c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test054) {
    performNormalizeTestFromManifest("#test054c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test055) {
    performNormalizeTestFromManifest("#test055c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test056) {
    performNormalizeTestFromManifest("#test056c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test057) {
    performNormalizeTestFromManifest("#test057c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test058) {
    performNormalizeTestFromManifest("#test058c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test059) {
    performNormalizeTestFromManifest("#test059c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test060) {
    performNormalizeTestFromManifest("#test060c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test061) {
    performNormalizeTestFromManifest("#test061c");
}

TEST_F(JsonLdProcessorNormalizeRdfc10Test, normalize_rdfc10_test062) {
    performNormalizeTestFromManifest("#test062c");
}

