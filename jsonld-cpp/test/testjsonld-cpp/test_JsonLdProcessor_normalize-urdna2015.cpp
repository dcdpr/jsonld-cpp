#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/JsonLdError.h>
#include <jsonld-cpp/CanonicalizationProcessor.h>
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
class JsonLdProcessorCanonicalizeUrdna2015Test : public ::testing::Test {
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

    static void performCanonicalizeTest(TestCase &testCase) {
        JsonLdOptions options = testCase.getOptions();

        std::cout << "Id: " << testCase.id << std::endl;
        std::cout << "Name: " << testCase.name << std::endl;
        if(!testCase.options.specVersion.empty())
            std::cout << "SpecVersion: " << testCase.options.specVersion << std::endl;

        std::string canonicalized;
        try {
            canonicalized = CanonicalizationProcessor::canonicalize(testCase.input, options);
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

        EXPECT_EQ(canonicalized, expected);

    }

    static void performCanonicalizeTestFromAlternateManifest(const std::string& testName, const std::string& manifestName) {

        std::string resource_dir = TEST_RESOURCE_DIR;
        UrdnaManifestLoader manifestLoader(
                resource_dir + "test_data/",
                manifestName);
        std::map<std::string, TestCase> localTestCases = manifestLoader.load();

        auto testCase = localTestCases.at(testName);

        performCanonicalizeTest(testCase);
    }

    void performCanonicalizeTestFromManifest(const std::string& testName, const std::string& manifestName= "") {

        if(!manifestName.empty())
            return performCanonicalizeTestFromAlternateManifest(testName, manifestName);

        auto testCase = testCases.at(testName);

        performCanonicalizeTest(testCase);
    }

};

std::string JsonLdProcessorCanonicalizeUrdna2015Test::mainManifestName = "canonicalize-manifest-urdna2015.jsonld";
std::map<std::string, TestCase> JsonLdProcessorCanonicalizeUrdna2015Test::testCases;


TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test001) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test001");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test002) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test002");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test003) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test003");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test004) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test004");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test005) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test005");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test006) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test006");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test007) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test007");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test008) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test008");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test009) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test009");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test010) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test010");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test011) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test011");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test012) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test012");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test013) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test013");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test014) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test014");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test015) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test015");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test016) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test016");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test017) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test017");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test018) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test018");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test019) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test019");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test020) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test020");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test021) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test021");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test022) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test022");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test023) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test023");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test024) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test024");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test025) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test025");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test026) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test026");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test027) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test027");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test028) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test028");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test029) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test029");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test030) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test030");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test031) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test031");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test032) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test032");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test033) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test033");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test034) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test034");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test035) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test035");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test036) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test036");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test037) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test037");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test038) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test038");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test039) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test039");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test040) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test040");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test041) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test041");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test042) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test042");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test043) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test043");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test044) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test044");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test045) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test045");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test046) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test046");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test047) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test047");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test048) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test048");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test049) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test049");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test050) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test050");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test051) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test051");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test052) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test052");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test053) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test053");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test054) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test054");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test055) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test055");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test056) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test056");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test057) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test057");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test058) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test058");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test059) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test059");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test060) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test060");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test061) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test061");
}

TEST_F(JsonLdProcessorCanonicalizeUrdna2015Test, canonicalize_manifest_urdna2015test062) {
    performCanonicalizeTestFromManifest("manifest-urdna2015#test062");
}

