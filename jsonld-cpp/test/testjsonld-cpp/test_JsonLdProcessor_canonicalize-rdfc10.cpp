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
class JsonLdProcessorCanonicalizeRdfc10Test : public ::testing::Test {
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

std::string JsonLdProcessorCanonicalizeRdfc10Test::mainManifestName = "canonicalize-manifest-rdfc10.jsonld";
std::map<std::string, TestCase> JsonLdProcessorCanonicalizeRdfc10Test::testCases;


TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test001) {
    performCanonicalizeTestFromManifest("#test001c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test002) {
    performCanonicalizeTestFromManifest("#test002c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test003) {
    performCanonicalizeTestFromManifest("#test003c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test004) {
    performCanonicalizeTestFromManifest("#test004c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test005) {
    performCanonicalizeTestFromManifest("#test005c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test006) {
    performCanonicalizeTestFromManifest("#test006c");
}

// #test007c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test008) {
    performCanonicalizeTestFromManifest("#test008c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test009) {
    performCanonicalizeTestFromManifest("#test009c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test010) {
    performCanonicalizeTestFromManifest("#test010c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test011) {
    performCanonicalizeTestFromManifest("#test011c");
}

// #test012c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test013) {
    performCanonicalizeTestFromManifest("#test013c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test014) {
    performCanonicalizeTestFromManifest("#test014c");
}

// #test0015c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test016) {
    performCanonicalizeTestFromManifest("#test016c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test017) {
    performCanonicalizeTestFromManifest("#test017c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test018) {
    performCanonicalizeTestFromManifest("#test018c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test019) {
    performCanonicalizeTestFromManifest("#test019c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test020) {
    performCanonicalizeTestFromManifest("#test020c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test021) {
    performCanonicalizeTestFromManifest("#test021c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test022) {
    performCanonicalizeTestFromManifest("#test022c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test023) {
    performCanonicalizeTestFromManifest("#test023c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test024) {
    performCanonicalizeTestFromManifest("#test024c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test025) {
    performCanonicalizeTestFromManifest("#test025c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test026) {
    performCanonicalizeTestFromManifest("#test026c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test027) {
    performCanonicalizeTestFromManifest("#test027c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test028) {
    performCanonicalizeTestFromManifest("#test028c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test029) {
    performCanonicalizeTestFromManifest("#test029c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test030) {
    performCanonicalizeTestFromManifest("#test030c");
}

// #test0031c does not exist

// #test0032c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test033) {
    performCanonicalizeTestFromManifest("#test033c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test034) {
    performCanonicalizeTestFromManifest("#test034c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test035) {
    performCanonicalizeTestFromManifest("#test035c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test036) {
    performCanonicalizeTestFromManifest("#test036c");
}

// #test037c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test038) {
    performCanonicalizeTestFromManifest("#test038c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test039) {
    performCanonicalizeTestFromManifest("#test039c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test040) {
    performCanonicalizeTestFromManifest("#test040c");
}

// #test0041c does not exist

// #test0042c does not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test043) {
    performCanonicalizeTestFromManifest("#test043c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test044) {
    performCanonicalizeTestFromManifest("#test044c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test045) {
    performCanonicalizeTestFromManifest("#test045c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test046) {
    performCanonicalizeTestFromManifest("#test046c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test047) {
    performCanonicalizeTestFromManifest("#test047c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test048) {
    performCanonicalizeTestFromManifest("#test048c");
}

// #test0049c - #test0052c do not exist

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test053) {
    performCanonicalizeTestFromManifest("#test053c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test054) {
    performCanonicalizeTestFromManifest("#test054c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test055) {
    performCanonicalizeTestFromManifest("#test055c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test056) {
    performCanonicalizeTestFromManifest("#test056c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test057) {
    performCanonicalizeTestFromManifest("#test057c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test058) {
    performCanonicalizeTestFromManifest("#test058c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test059) {
    performCanonicalizeTestFromManifest("#test059c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test060) {
    performCanonicalizeTestFromManifest("#test060c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test061) {
    performCanonicalizeTestFromManifest("#test061c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test062) {
    performCanonicalizeTestFromManifest("#test062c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test063) {
    performCanonicalizeTestFromManifest("#test063c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test064) {
    performCanonicalizeTestFromManifest("#test064c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test065) {
    performCanonicalizeTestFromManifest("#test065c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test066) {
    performCanonicalizeTestFromManifest("#test066c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test067) {
    performCanonicalizeTestFromManifest("#test067c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test068) {
    performCanonicalizeTestFromManifest("#test068c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test069) {
    performCanonicalizeTestFromManifest("#test069c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test070) {
    performCanonicalizeTestFromManifest("#test070c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test071) {
    performCanonicalizeTestFromManifest("#test071c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test072) {
    performCanonicalizeTestFromManifest("#test072c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test073) {
    performCanonicalizeTestFromManifest("#test073c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test074) {
    //performCanonicalizeTestFromManifest("#test074c");
}

TEST_F(JsonLdProcessorCanonicalizeRdfc10Test, canonicalize_rdfc10_test075) {
    performCanonicalizeTestFromManifest("#test075c");
}

