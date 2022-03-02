#pragma ide diagnostic ignored "cert-err58-cpp"
#include "JsonLdProcessor.h"
#include "testHelpers.h"
#include "UrdnaManifestLoader.h"
#include "RDFDatasetUtils.h"
#include <fstream>

using nlohmann::json;

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <FileLoader.h>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// test suite fixture class
class JsonLdProcessorNormalizeUrdna2015Test : public ::testing::Test {
public:
    static void SetUpTestCase() {
        UrdnaManifestLoader manifestLoader(
                resolvePath("test/testjsonld-cpp/test_data/"),
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
                RDFDatasetUtils::toNQuads(expectedDocument->getRDFContent());

        EXPECT_EQ(normalized, expected);

        std::cout << "  Actual normalized: " << normalized << std::endl;
        std::cout << "Expected normalized: " << expected << std::endl;
    }

    void performNormalizeTestFromAlternateManifest(const std::string& testName, const std::string& manifestName) {

        UrdnaManifestLoader manifestLoader(
                resolvePath("test/testjsonld-cpp/test_data/"),
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

std::string JsonLdProcessorNormalizeUrdna2015Test::mainManifestName = "normalize-manifest-urdna2015.jsonld";
std::map<std::string, TestCase> JsonLdProcessorNormalizeUrdna2015Test::testCases;


TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test001) {
    performNormalizeTestFromManifest("manifest-urdna2015#test001");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test002) {
    performNormalizeTestFromManifest("manifest-urdna2015#test002");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test003) {
    performNormalizeTestFromManifest("manifest-urdna2015#test003");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test004) {
    performNormalizeTestFromManifest("manifest-urdna2015#test004");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test005) {
    performNormalizeTestFromManifest("manifest-urdna2015#test005");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test006) {
    performNormalizeTestFromManifest("manifest-urdna2015#test006");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test007) {
    performNormalizeTestFromManifest("manifest-urdna2015#test007");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test008) {
    performNormalizeTestFromManifest("manifest-urdna2015#test008");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test009) {
    performNormalizeTestFromManifest("manifest-urdna2015#test009");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test010) {
    performNormalizeTestFromManifest("manifest-urdna2015#test010");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test011) {
    performNormalizeTestFromManifest("manifest-urdna2015#test011");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test012) {
    performNormalizeTestFromManifest("manifest-urdna2015#test012");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test013) {
    performNormalizeTestFromManifest("manifest-urdna2015#test013");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test014) {
    performNormalizeTestFromManifest("manifest-urdna2015#test014");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test015) {
    performNormalizeTestFromManifest("manifest-urdna2015#test015");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test016) {
    performNormalizeTestFromManifest("manifest-urdna2015#test016");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test017) {
    performNormalizeTestFromManifest("manifest-urdna2015#test017");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test018) {
    performNormalizeTestFromManifest("manifest-urdna2015#test018");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test019) {
    performNormalizeTestFromManifest("manifest-urdna2015#test019");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test020) {
    performNormalizeTestFromManifest("manifest-urdna2015#test020");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test021) {
    performNormalizeTestFromManifest("manifest-urdna2015#test021");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test022) {
    performNormalizeTestFromManifest("manifest-urdna2015#test022");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test023) {
    performNormalizeTestFromManifest("manifest-urdna2015#test023");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test024) {
    performNormalizeTestFromManifest("manifest-urdna2015#test024");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test025) {
    performNormalizeTestFromManifest("manifest-urdna2015#test025");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test026) {
    performNormalizeTestFromManifest("manifest-urdna2015#test026");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test027) {
    performNormalizeTestFromManifest("manifest-urdna2015#test027");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test028) {
    performNormalizeTestFromManifest("manifest-urdna2015#test028");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test029) {
    performNormalizeTestFromManifest("manifest-urdna2015#test029");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test030) {
    performNormalizeTestFromManifest("manifest-urdna2015#test030");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test031) {
    performNormalizeTestFromManifest("manifest-urdna2015#test031");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test032) {
    performNormalizeTestFromManifest("manifest-urdna2015#test032");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test033) {
    performNormalizeTestFromManifest("manifest-urdna2015#test033");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test034) {
    performNormalizeTestFromManifest("manifest-urdna2015#test034");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test035) {
    performNormalizeTestFromManifest("manifest-urdna2015#test035");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test036) {
    performNormalizeTestFromManifest("manifest-urdna2015#test036");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test037) {
    performNormalizeTestFromManifest("manifest-urdna2015#test037");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test038) {
    performNormalizeTestFromManifest("manifest-urdna2015#test038");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test039) {
    performNormalizeTestFromManifest("manifest-urdna2015#test039");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test040) {
    performNormalizeTestFromManifest("manifest-urdna2015#test040");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test041) {
    performNormalizeTestFromManifest("manifest-urdna2015#test041");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test042) {
    performNormalizeTestFromManifest("manifest-urdna2015#test042");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test043) {
    performNormalizeTestFromManifest("manifest-urdna2015#test043");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test044) {
    performNormalizeTestFromManifest("manifest-urdna2015#test044");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test045) {
    performNormalizeTestFromManifest("manifest-urdna2015#test045");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test046) {
    performNormalizeTestFromManifest("manifest-urdna2015#test046");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test047) {
    performNormalizeTestFromManifest("manifest-urdna2015#test047");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test048) {
    performNormalizeTestFromManifest("manifest-urdna2015#test048");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test049) {
    performNormalizeTestFromManifest("manifest-urdna2015#test049");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test050) {
    performNormalizeTestFromManifest("manifest-urdna2015#test050");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test051) {
    performNormalizeTestFromManifest("manifest-urdna2015#test051");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test052) {
    performNormalizeTestFromManifest("manifest-urdna2015#test052");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test053) {
    performNormalizeTestFromManifest("manifest-urdna2015#test053");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test054) {
    performNormalizeTestFromManifest("manifest-urdna2015#test054");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test055) {
    performNormalizeTestFromManifest("manifest-urdna2015#test055");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test056) {
    performNormalizeTestFromManifest("manifest-urdna2015#test056");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test057) {
    performNormalizeTestFromManifest("manifest-urdna2015#test057");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test058) {
    performNormalizeTestFromManifest("manifest-urdna2015#test058");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test059) {
    performNormalizeTestFromManifest("manifest-urdna2015#test059");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test060) {
    performNormalizeTestFromManifest("manifest-urdna2015#test060");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test061) {
    performNormalizeTestFromManifest("manifest-urdna2015#test061");
}

TEST_F(JsonLdProcessorNormalizeUrdna2015Test, normalize_manifest_urdna2015test062) {
    performNormalizeTestFromManifest("manifest-urdna2015#test062");
}

