#pragma ide diagnostic ignored "cert-err58-cpp"
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
#include <FileLoader.h>
#include <RDFDatasetUtils.h>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// test suite fixture class
class JsonLdProcessorNormalizeTest : public ::testing::Test {
public:
    static void SetUpTestCase() {
        ManifestLoader manifestLoader(
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
            std::cout << "SpecVersion: " << testCase.options.specVersion;
        if(testCase.options.specVersion == "json-ld-1.1") {
            std::cout << " ...skipping for now." << std::endl;
            SUCCEED();
            return;
        }
        else
            std::cout << std::endl;

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
        }
        catch (std::runtime_error &e) {
            FAIL() << e.what();
        }

        std::unique_ptr<RemoteDocument> expectedDocument =
                options.getDocumentLoader()->loadDocument(testCase.expect);

        // todo: too many conversions? maybe JsonLdProcessor::normalize should return RDFDataset
        // so we can compare that directly to getRDFContent() rather than converting both
        // to strings for the compare?
        const std::string & expected =
                RDFDatasetUtils::toNQuads(expectedDocument->getRDFContent());

        EXPECT_EQ(normalized, expected);

        std::cout << "  Actual normalized: " << normalized << std::endl;
        std::cout << "Expected normalized: " << expected << std::endl;
    }

    void performNormalizeTestFromAlternateManifest(const std::string& testName, const std::string& manifestName) {

        ManifestLoader manifestLoader(
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

std::string JsonLdProcessorNormalizeTest::mainManifestName = "normalize-manifest.jsonld";
std::map<std::string, TestCase> JsonLdProcessorNormalizeTest::testCases;


TEST_F(JsonLdProcessorNormalizeTest, normalize_t0001) {
    performNormalizeTestFromManifest("#t0001");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0002) {
    performNormalizeTestFromManifest("#t0002");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0003) {
    performNormalizeTestFromManifest("#t0003");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0004) {
    performNormalizeTestFromManifest("#t0004");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0005) {
    performNormalizeTestFromManifest("#t0005");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0006) {
    performNormalizeTestFromManifest("#t0006");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0007) {
    performNormalizeTestFromManifest("#t0007");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0008) {
    performNormalizeTestFromManifest("#t0008");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0009) {
    performNormalizeTestFromManifest("#t0009");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0010) {
    performNormalizeTestFromManifest("#t0010");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0011) {
    performNormalizeTestFromManifest("#t0011");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0012) {
    performNormalizeTestFromManifest("#t0012");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0013) {
    performNormalizeTestFromManifest("#t0013");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0014) {
    performNormalizeTestFromManifest("#t0014");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0015) {
    performNormalizeTestFromManifest("#t0015");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0016) {
    performNormalizeTestFromManifest("#t0016");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0017) {
    performNormalizeTestFromManifest("#t0017");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0018) {
    performNormalizeTestFromManifest("#t0018");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0019) {
    performNormalizeTestFromManifest("#t0019");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0020) {
    performNormalizeTestFromManifest("#t0020");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0021) {
    performNormalizeTestFromManifest("#t0021");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0022) {
    performNormalizeTestFromManifest("#t0022");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0023) {
    performNormalizeTestFromManifest("#t0023");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0024) {
    performNormalizeTestFromManifest("#t0024");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0025) {
    performNormalizeTestFromManifest("#t0025");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0026) {
    performNormalizeTestFromManifest("#t0026");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0027) {
    performNormalizeTestFromManifest("#t0027");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0028) {
    performNormalizeTestFromManifest("#t0028");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0029) {
    performNormalizeTestFromManifest("#t0029");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0030) {
    performNormalizeTestFromManifest("#t0030");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0031) {
    performNormalizeTestFromManifest("#t0031");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0032) {
    performNormalizeTestFromManifest("#t0032");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0033) {
    performNormalizeTestFromManifest("#t0033");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0034) {
    performNormalizeTestFromManifest("#t0034");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0035) {
    performNormalizeTestFromManifest("#t0035");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0036) {
    performNormalizeTestFromManifest("#t0036");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0037) {
    performNormalizeTestFromManifest("#t0037");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0038) {
    performNormalizeTestFromManifest("#t0038");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0039) {
    performNormalizeTestFromManifest("#t0039");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0040) {
    performNormalizeTestFromManifest("#t0040");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0041) {
    performNormalizeTestFromManifest("#t0041");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0042) {
    performNormalizeTestFromManifest("#t0042");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0043) {
    performNormalizeTestFromManifest("#t0043");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0044) {
    performNormalizeTestFromManifest("#t0044");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0045) {
    performNormalizeTestFromManifest("#t0045");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0046) {
    performNormalizeTestFromManifest("#t0046");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0047) {
    performNormalizeTestFromManifest("#t0047");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0048) {
    performNormalizeTestFromManifest("#t0048");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0049) {
    performNormalizeTestFromManifest("#t0049");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0050) {
    performNormalizeTestFromManifest("#t0050");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0051) {
    performNormalizeTestFromManifest("#t0051");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0052) {
    performNormalizeTestFromManifest("#t0052");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0053) {
    performNormalizeTestFromManifest("#t0053");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0054) {
    performNormalizeTestFromManifest("#t0054");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0055) {
    performNormalizeTestFromManifest("#t0055");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0056) {
    performNormalizeTestFromManifest("#t0056");
}

TEST_F(JsonLdProcessorNormalizeTest, normalize_t0057) {
    performNormalizeTestFromManifest("#t0057");
}



