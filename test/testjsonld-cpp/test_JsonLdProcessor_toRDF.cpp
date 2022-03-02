#pragma ide diagnostic ignored "cert-err58-cpp"
#include "JsonLdProcessor.h"
#include "testHelpers.h"
#include "ManifestLoader.h"
#include "FileLoader.h"
#include "RDFDatasetUtils.h"
#include <fstream>

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
class JsonLdProcessorToRDFTest : public ::testing::Test {
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

    static void performToRDFTest(TestCase &testCase) {
        JsonLdOptions options = testCase.getOptions();

        std::cout << "Id: " << testCase.id << std::endl;
        std::cout << "Name: " << testCase.name << std::endl;
        if(!testCase.options.specVersion.empty())
            std::cout << "SpecVersion: " << testCase.options.specVersion << std::endl;


        try {
            RDF::RDFDataset actual = JsonLdProcessor::toRDF(testCase.input, options);
            if (testCase.type.count("jld:PositiveSyntaxTest")) {
                SUCCEED();
                return;
            }

            std::unique_ptr<RemoteDocument> expectedDocument =
                    options.getDocumentLoader()->loadDocument(testCase.expect);
            RDF::RDFDataset expected = expectedDocument->getRDFContent();

            EXPECT_TRUE(areIsomorphic(actual, expected));

            std::cout << "  Actual RDF: " << RDFDatasetUtils::toNQuads(actual) << std::endl;
            std::cout << "Expected RDF: " << RDFDatasetUtils::toNQuads(expected) << std::endl;

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

    }

    void performToRDFTestFromAlternateManifest(const std::string& testName, const std::string& manifestName) {

        ManifestLoader manifestLoader(
                resolvePath("test/testjsonld-cpp/test_data/"),
                manifestName);
        std::map<std::string, TestCase> localTestCases = manifestLoader.load();

        auto testCase = localTestCases.at(testName);

        performToRDFTest(testCase);
    }

    void performToRDFTestFromManifest(const std::string& testName, const std::string& manifestName="") {

        if(!manifestName.empty())
            return performToRDFTestFromAlternateManifest(testName, manifestName);

        auto testCase = testCases.at(testName);

        performToRDFTest(testCase);
    }

};

std::string JsonLdProcessorToRDFTest::mainManifestName = "toRdf-manifest.jsonld";
std::map<std::string, TestCase> JsonLdProcessorToRDFTest::testCases;


TEST_F(JsonLdProcessorToRDFTest, toRdf_t0001) {
    performToRDFTestFromManifest("#t0001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0002) {
    performToRDFTestFromManifest("#t0002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0003) {
    performToRDFTestFromManifest("#t0003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0004) {
    performToRDFTestFromManifest("#t0004");
}

// todo: disable until I have better utf8 char handling?
TEST_F(JsonLdProcessorToRDFTest, toRdf_t0005) {
    performToRDFTestFromManifest("#t0005");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0006) {
    performToRDFTestFromManifest("#t0006");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0007) {
    performToRDFTestFromManifest("#t0007");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0008) {
    performToRDFTestFromManifest("#t0008");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0009) {
    performToRDFTestFromManifest("#t0009");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0010) {
    performToRDFTestFromManifest("#t0010");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0011) {
    performToRDFTestFromManifest("#t0011");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0012) {
    performToRDFTestFromManifest("#t0012");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0013) {
    performToRDFTestFromManifest("#t0013");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0014) {
    performToRDFTestFromManifest("#t0014");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0015) {
    performToRDFTestFromManifest("#t0015");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0016) {
    performToRDFTestFromManifest("#t0016");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0017) {
    performToRDFTestFromManifest("#t0017");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0018) {
    performToRDFTestFromManifest("#t0018");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0019) {
    performToRDFTestFromManifest("#t0019");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0020) {
    performToRDFTestFromManifest("#t0020");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0022) {
    performToRDFTestFromManifest("#t0022");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0023) {
    performToRDFTestFromManifest("#t0023");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0024) {
    performToRDFTestFromManifest("#t0024");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0025) {
    performToRDFTestFromManifest("#t0025");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0026) {
    performToRDFTestFromManifest("#t0026");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0027) {
    performToRDFTestFromManifest("#t0027");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0028) {
    performToRDFTestFromManifest("#t0028");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0029) {
    performToRDFTestFromManifest("#t0029");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0030) {
    performToRDFTestFromManifest("#t0030");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0031) {
    performToRDFTestFromManifest("#t0031");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0032) {
    performToRDFTestFromManifest("#t0032");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0033) {
    performToRDFTestFromManifest("#t0033");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0034) {
    performToRDFTestFromManifest("#t0034");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0035) {
    performToRDFTestFromManifest("#t0035");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0036) {
    performToRDFTestFromManifest("#t0036");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0113) {
    performToRDFTestFromManifest("#t0113");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0114) {
    performToRDFTestFromManifest("#t0114");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0115) {
    performToRDFTestFromManifest("#t0115");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0116) {
    performToRDFTestFromManifest("#t0116");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0117) {
    performToRDFTestFromManifest("#t0117");
}

// Test output file has blank nodes as predicates, and they are not supported. We
// may want to disable, but should keep failing test for test implementation report.
TEST_F(JsonLdProcessorToRDFTest, toRdf_t0118) {
    performToRDFTestFromManifest("#t0118");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0119) {
    performToRDFTestFromManifest("#t0119");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0120) {
    performToRDFTestFromManifest("#t0120");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0121) {
    performToRDFTestFromManifest("#t0121");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0122) {
    performToRDFTestFromManifest("#t0122");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0123) {
    performToRDFTestFromManifest("#t0123");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0124) {
    performToRDFTestFromManifest("#t0124");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0125) {
    performToRDFTestFromManifest("#t0125");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0126) {
    performToRDFTestFromManifest("#t0126");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0127) {
    performToRDFTestFromManifest("#t0127");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0128) {
    performToRDFTestFromManifest("#t0128");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0129) {
    performToRDFTestFromManifest("#t0129");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0130) {
    performToRDFTestFromManifest("#t0130");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0131) {
    performToRDFTestFromManifest("#t0131");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_t0132) {
    performToRDFTestFromManifest("#t0132");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc001) {
    performToRDFTestFromManifest("#tc001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc002) {
    performToRDFTestFromManifest("#tc002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc003) {
    performToRDFTestFromManifest("#tc003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc004) {
    performToRDFTestFromManifest("#tc004");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc005) {
    performToRDFTestFromManifest("#tc005");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc006) {
    performToRDFTestFromManifest("#tc006");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc007) {
    performToRDFTestFromManifest("#tc007");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc008) {
    performToRDFTestFromManifest("#tc008");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc009) {
    performToRDFTestFromManifest("#tc009");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc010) {
    performToRDFTestFromManifest("#tc010");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc011) {
    performToRDFTestFromManifest("#tc011");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc012) {
    performToRDFTestFromManifest("#tc012");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc013) {
    performToRDFTestFromManifest("#tc013");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc014) {
    performToRDFTestFromManifest("#tc014");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc015) {
    performToRDFTestFromManifest("#tc015");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc016) {
    performToRDFTestFromManifest("#tc016");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc017) {
    performToRDFTestFromManifest("#tc017");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc018) {
    performToRDFTestFromManifest("#tc018");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc019) {
    performToRDFTestFromManifest("#tc019");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc020) {
    performToRDFTestFromManifest("#tc020");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc021) {
    performToRDFTestFromManifest("#tc021");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc022) {
    performToRDFTestFromManifest("#tc022");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc023) {
    performToRDFTestFromManifest("#tc023");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc024) {
    performToRDFTestFromManifest("#tc024");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc025) {
    performToRDFTestFromManifest("#tc025");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc026) {
    performToRDFTestFromManifest("#tc026");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc027) {
    performToRDFTestFromManifest("#tc027");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc028) {
    performToRDFTestFromManifest("#tc028");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc029) {
    performToRDFTestFromManifest("#tc029");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc030) {
    performToRDFTestFromManifest("#tc030");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc031) {
    performToRDFTestFromManifest("#tc031");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc032) {
    performToRDFTestFromManifest("#tc032");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc033) {
    performToRDFTestFromManifest("#tc033");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc034) {
    performToRDFTestFromManifest("#tc034");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc035) {
    performToRDFTestFromManifest("#tc035");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tc036) {
    performToRDFTestFromManifest("#tc036");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi01) {
    performToRDFTestFromManifest("#tdi01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi02) {
    performToRDFTestFromManifest("#tdi02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi03) {
    performToRDFTestFromManifest("#tdi03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi04) {
    performToRDFTestFromManifest("#tdi04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi05) {
    performToRDFTestFromManifest("#tdi05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi06) {
    performToRDFTestFromManifest("#tdi06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi07) {
    performToRDFTestFromManifest("#tdi07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi08) {
    performToRDFTestFromManifest("#tdi08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi09) {
    performToRDFTestFromManifest("#tdi09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi10) {
    performToRDFTestFromManifest("#tdi10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi11) {
    performToRDFTestFromManifest("#tdi11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tdi12) {
    performToRDFTestFromManifest("#tdi12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te001) {
    performToRDFTestFromManifest("#te001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te002) {
    performToRDFTestFromManifest("#te002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te003) {
    performToRDFTestFromManifest("#te003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te004) {
    performToRDFTestFromManifest("#te004");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te005) {
    performToRDFTestFromManifest("#te005");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te006) {
    performToRDFTestFromManifest("#te006");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te007) {
    performToRDFTestFromManifest("#te007");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te008) {
    performToRDFTestFromManifest("#te008");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te009) {
    performToRDFTestFromManifest("#te009");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te010) {
    performToRDFTestFromManifest("#te010");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te011) {
    performToRDFTestFromManifest("#te011");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te012) {
    performToRDFTestFromManifest("#te012");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te013) {
    performToRDFTestFromManifest("#te013");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te014) {
    performToRDFTestFromManifest("#te014");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te015) {
    performToRDFTestFromManifest("#te015");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te016) {
    performToRDFTestFromManifest("#te016");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te017) {
    performToRDFTestFromManifest("#te017");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te018) {
    performToRDFTestFromManifest("#te018");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te019) {
    performToRDFTestFromManifest("#te019");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te020) {
    performToRDFTestFromManifest("#te020");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te021) {
    performToRDFTestFromManifest("#te021");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te022) {
    performToRDFTestFromManifest("#te022");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te023) {
    performToRDFTestFromManifest("#te023");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te024) {
    performToRDFTestFromManifest("#te024");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te025) {
    performToRDFTestFromManifest("#te025");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te026) {
    performToRDFTestFromManifest("#te026");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te027) {
    performToRDFTestFromManifest("#te027");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te028) {
    performToRDFTestFromManifest("#te028");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te029) {
    performToRDFTestFromManifest("#te029");
}

// todo: disable until I have better utf8 char handling?
TEST_F(JsonLdProcessorToRDFTest, toRdf_te030) {
    performToRDFTestFromManifest("#te030");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te031) {
    performToRDFTestFromManifest("#te031");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te032) {
    performToRDFTestFromManifest("#te032");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te033) {
    performToRDFTestFromManifest("#te033");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te034) {
    performToRDFTestFromManifest("#te034");
}

// todo: disable until I have better utf8 char handling?
TEST_F(JsonLdProcessorToRDFTest, toRdf_te035) {
    performToRDFTestFromManifest("#te035");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te036) {
    performToRDFTestFromManifest("#te036");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te037) {
    performToRDFTestFromManifest("#te037");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te038) {
    performToRDFTestFromManifest("#te038");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te039) {
    performToRDFTestFromManifest("#te039");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te040) {
    performToRDFTestFromManifest("#te040");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te041) {
    performToRDFTestFromManifest("#te041");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te042) {
    performToRDFTestFromManifest("#te042");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te043) {
    performToRDFTestFromManifest("#te043");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te044) {
    performToRDFTestFromManifest("#te044");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te045) {
    performToRDFTestFromManifest("#te045");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te046) {
    performToRDFTestFromManifest("#te046");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te047) {
    performToRDFTestFromManifest("#te047");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te048) {
    performToRDFTestFromManifest("#te048");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te049) {
    performToRDFTestFromManifest("#te049");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te050) {
    performToRDFTestFromManifest("#te050");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te051) {
    performToRDFTestFromManifest("#te051");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te052) {
    performToRDFTestFromManifest("#te052");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te053) {
    performToRDFTestFromManifest("#te053");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te054) {
    performToRDFTestFromManifest("#te054");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te055) {
    performToRDFTestFromManifest("#te055");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te056) {
    performToRDFTestFromManifest("#te056");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te057) {
    performToRDFTestFromManifest("#te057");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te058) {
    performToRDFTestFromManifest("#te058");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te059) {
    performToRDFTestFromManifest("#te059");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te060) {
    performToRDFTestFromManifest("#te060");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te061) {
    performToRDFTestFromManifest("#te061");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te062) {
    performToRDFTestFromManifest("#te062");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te063) {
    performToRDFTestFromManifest("#te063");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te064) {
    performToRDFTestFromManifest("#te064");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te065) {
    performToRDFTestFromManifest("#te065");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te066) {
    performToRDFTestFromManifest("#te066");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te067) {
    performToRDFTestFromManifest("#te067");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te068) {
    performToRDFTestFromManifest("#te068");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te069) {
    performToRDFTestFromManifest("#te069");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te070) {
    performToRDFTestFromManifest("#te070");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te071) {
    performToRDFTestFromManifest("#te071");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te072) {
    performToRDFTestFromManifest("#te072");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te073) {
    performToRDFTestFromManifest("#te073");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te074) {
    performToRDFTestFromManifest("#te074");
}

// Test output file has blank nodes as predicates, and they are not supported
TEST_F(JsonLdProcessorToRDFTest, toRdf_te075) {
    performToRDFTestFromManifest("#te075");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te076) {
    performToRDFTestFromManifest("#te076");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te077) {
    performToRDFTestFromManifest("#te077");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te078) {
    performToRDFTestFromManifest("#te078");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te079) {
    performToRDFTestFromManifest("#te079");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te080) {
    performToRDFTestFromManifest("#te080");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te081) {
    performToRDFTestFromManifest("#te081");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te082) {
    performToRDFTestFromManifest("#te082");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te083) {
    performToRDFTestFromManifest("#te083");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te084) {
    performToRDFTestFromManifest("#te084");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te085) {
    performToRDFTestFromManifest("#te085");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te086) {
    performToRDFTestFromManifest("#te086");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te087) {
    performToRDFTestFromManifest("#te087");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te088) {
    performToRDFTestFromManifest("#te088");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te089) {
    performToRDFTestFromManifest("#te089");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te090) {
    performToRDFTestFromManifest("#te090");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te091) {
    performToRDFTestFromManifest("#te091");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te092) {
    performToRDFTestFromManifest("#te092");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te093) {
    performToRDFTestFromManifest("#te093");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te094) {
    performToRDFTestFromManifest("#te094");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te095) {
    performToRDFTestFromManifest("#te095");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te096) {
    performToRDFTestFromManifest("#te096");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te097) {
    performToRDFTestFromManifest("#te097");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te098) {
    performToRDFTestFromManifest("#te098");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te099) {
    performToRDFTestFromManifest("#te099");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te100) {
    performToRDFTestFromManifest("#te100");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te101) {
    performToRDFTestFromManifest("#te101");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te102) {
    performToRDFTestFromManifest("#te102");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te103) {
    performToRDFTestFromManifest("#te103");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te104) {
    performToRDFTestFromManifest("#te104");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te105) {
    performToRDFTestFromManifest("#te105");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te106) {
    performToRDFTestFromManifest("#te106");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te107) {
    performToRDFTestFromManifest("#te107");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te108) {
    performToRDFTestFromManifest("#te108");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te109) {
    performToRDFTestFromManifest("#te109");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te110) {
    performToRDFTestFromManifest("#te110");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te111) {
    performToRDFTestFromManifest("#te111");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te112) {
    performToRDFTestFromManifest("#te112");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te113) {
    performToRDFTestFromManifest("#te113");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te114) {
    performToRDFTestFromManifest("#te114");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te115) {
    performToRDFTestFromManifest("#te115");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te116) {
    performToRDFTestFromManifest("#te116");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te117) {
    performToRDFTestFromManifest("#te117");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te118) {
    performToRDFTestFromManifest("#te118");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te119) {
    performToRDFTestFromManifest("#te119");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te120) {
    performToRDFTestFromManifest("#te120");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te121) {
    performToRDFTestFromManifest("#te121");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te122) {
    performToRDFTestFromManifest("#te122");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te123) {
    performToRDFTestFromManifest("#te123");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te124) {
    performToRDFTestFromManifest("#te124");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te125) {
    performToRDFTestFromManifest("#te125");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te126) {
    performToRDFTestFromManifest("#te126");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te127) {
    performToRDFTestFromManifest("#te127");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te128) {
    performToRDFTestFromManifest("#te128");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te129) {
    performToRDFTestFromManifest("#te129");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_te130) {
    performToRDFTestFromManifest("#te130");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tec01) {
    performToRDFTestFromManifest("#tec01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tec02) {
    performToRDFTestFromManifest("#tec02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tem01) {
    performToRDFTestFromManifest("#tem01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten01) {
    performToRDFTestFromManifest("#ten01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten02) {
    performToRDFTestFromManifest("#ten02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten03) {
    performToRDFTestFromManifest("#ten03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten04) {
    performToRDFTestFromManifest("#ten04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten05) {
    performToRDFTestFromManifest("#ten05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ten06) {
    performToRDFTestFromManifest("#ten06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tep02) {
    performToRDFTestFromManifest("#tep02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tep03) {
    performToRDFTestFromManifest("#tep03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter01) {
    performToRDFTestFromManifest("#ter01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter02) {
    performToRDFTestFromManifest("#ter02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter03) {
    performToRDFTestFromManifest("#ter03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter04) {
    performToRDFTestFromManifest("#ter04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter05) {
    performToRDFTestFromManifest("#ter05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter06) {
    performToRDFTestFromManifest("#ter06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter07) {
    performToRDFTestFromManifest("#ter07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter08) {
    performToRDFTestFromManifest("#ter08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter09) {
    performToRDFTestFromManifest("#ter09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter10) {
    performToRDFTestFromManifest("#ter10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter11) {
    performToRDFTestFromManifest("#ter11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter12) {
    performToRDFTestFromManifest("#ter12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter13) {
    performToRDFTestFromManifest("#ter13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter14) {
    performToRDFTestFromManifest("#ter14");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter15) {
    performToRDFTestFromManifest("#ter15");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter17) {
    performToRDFTestFromManifest("#ter17");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter18) {
    performToRDFTestFromManifest("#ter18");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter19) {
    performToRDFTestFromManifest("#ter19");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter20) {
    performToRDFTestFromManifest("#ter20");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter21) {
    performToRDFTestFromManifest("#ter21");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter22) {
    performToRDFTestFromManifest("#ter22");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter23) {
    performToRDFTestFromManifest("#ter23");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter24) {
    performToRDFTestFromManifest("#ter24");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter25) {
    performToRDFTestFromManifest("#ter25");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter26) {
    performToRDFTestFromManifest("#ter26");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter27) {
    performToRDFTestFromManifest("#ter27");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter28) {
    performToRDFTestFromManifest("#ter28");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter29) {
    performToRDFTestFromManifest("#ter29");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter30) {
    performToRDFTestFromManifest("#ter30");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter31) {
    performToRDFTestFromManifest("#ter31");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter32) {
    performToRDFTestFromManifest("#ter32");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter33) {
    performToRDFTestFromManifest("#ter33");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter34) {
    performToRDFTestFromManifest("#ter34");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter35) {
    performToRDFTestFromManifest("#ter35");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter36) {
    performToRDFTestFromManifest("#ter36");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter37) {
    performToRDFTestFromManifest("#ter37");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter38) {
    performToRDFTestFromManifest("#ter38");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter39) {
    performToRDFTestFromManifest("#ter39");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter40) {
    performToRDFTestFromManifest("#ter40");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter41) {
    performToRDFTestFromManifest("#ter41");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter42) {
    performToRDFTestFromManifest("#ter42");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter43) {
    performToRDFTestFromManifest("#ter43");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter44) {
    performToRDFTestFromManifest("#ter44");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter48) {
    performToRDFTestFromManifest("#ter48");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter49) {
    performToRDFTestFromManifest("#ter49");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter50) {
    performToRDFTestFromManifest("#ter50");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter51) {
    performToRDFTestFromManifest("#ter51");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter52) {
    performToRDFTestFromManifest("#ter52");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter53) {
    performToRDFTestFromManifest("#ter53");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter54) {
    performToRDFTestFromManifest("#ter54");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ter55) {
    performToRDFTestFromManifest("#ter55");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin01) {
    performToRDFTestFromManifest("#tin01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin02) {
    performToRDFTestFromManifest("#tin02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin03) {
    performToRDFTestFromManifest("#tin03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin04) {
    performToRDFTestFromManifest("#tin04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin05) {
    performToRDFTestFromManifest("#tin05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin06) {
    performToRDFTestFromManifest("#tin06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin07) {
    performToRDFTestFromManifest("#tin07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin08) {
    performToRDFTestFromManifest("#tin08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tin09) {
    performToRDFTestFromManifest("#tin09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs01) {
    performToRDFTestFromManifest("#tjs01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs02) {
    performToRDFTestFromManifest("#tjs02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs03) {
    performToRDFTestFromManifest("#tjs03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs04) {
    performToRDFTestFromManifest("#tjs04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs05) {
    performToRDFTestFromManifest("#tjs05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs06) {
    performToRDFTestFromManifest("#tjs06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs07) {
    performToRDFTestFromManifest("#tjs07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs08) {
    performToRDFTestFromManifest("#tjs08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs09) {
    performToRDFTestFromManifest("#tjs09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs10) {
    performToRDFTestFromManifest("#tjs10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs11) {
    performToRDFTestFromManifest("#tjs11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs12) {
    performToRDFTestFromManifest("#tjs12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs13) {
    performToRDFTestFromManifest("#tjs13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs14) {
    performToRDFTestFromManifest("#tjs14");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs15) {
    performToRDFTestFromManifest("#tjs15");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs16) {
    performToRDFTestFromManifest("#tjs16");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs17) {
    performToRDFTestFromManifest("#tjs17");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs18) {
    performToRDFTestFromManifest("#tjs18");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs19) {
    performToRDFTestFromManifest("#tjs19");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs20) {
    performToRDFTestFromManifest("#tjs20");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs21) {
    performToRDFTestFromManifest("#tjs21");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs22) {
    performToRDFTestFromManifest("#tjs22");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tjs23) {
    performToRDFTestFromManifest("#tjs23");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli01) {
    performToRDFTestFromManifest("#tli01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli02) {
    performToRDFTestFromManifest("#tli02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli03) {
    performToRDFTestFromManifest("#tli03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli04) {
    performToRDFTestFromManifest("#tli04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli05) {
    performToRDFTestFromManifest("#tli05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli06) {
    performToRDFTestFromManifest("#tli06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli07) {
    performToRDFTestFromManifest("#tli07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli08) {
    performToRDFTestFromManifest("#tli08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli09) {
    performToRDFTestFromManifest("#tli09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli10) {
    performToRDFTestFromManifest("#tli10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli11) {
    performToRDFTestFromManifest("#tli11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli12) {
    performToRDFTestFromManifest("#tli12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli13) {
    performToRDFTestFromManifest("#tli13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tli14) {
    performToRDFTestFromManifest("#tli14");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm001) {
    performToRDFTestFromManifest("#tm001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm002) {
    performToRDFTestFromManifest("#tm002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm003) {
    performToRDFTestFromManifest("#tm003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm004) {
    performToRDFTestFromManifest("#tm004");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm005) {
    performToRDFTestFromManifest("#tm005");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm006) {
    performToRDFTestFromManifest("#tm006");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm007) {
    performToRDFTestFromManifest("#tm007");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm008) {
    performToRDFTestFromManifest("#tm008");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm009) {
    performToRDFTestFromManifest("#tm009");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm010) {
    performToRDFTestFromManifest("#tm010");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm011) {
    performToRDFTestFromManifest("#tm011");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm012) {
    performToRDFTestFromManifest("#tm012");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm013) {
    performToRDFTestFromManifest("#tm013");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm014) {
    performToRDFTestFromManifest("#tm014");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm015) {
    performToRDFTestFromManifest("#tm015");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm016) {
    performToRDFTestFromManifest("#tm016");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm017) {
    performToRDFTestFromManifest("#tm017");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm018) {
    performToRDFTestFromManifest("#tm018");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm019) {
    performToRDFTestFromManifest("#tm019");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tm020) {
    performToRDFTestFromManifest("#tm020");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn001) {
    performToRDFTestFromManifest("#tn001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn002) {
    performToRDFTestFromManifest("#tn002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn003) {
    performToRDFTestFromManifest("#tn003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn004) {
    performToRDFTestFromManifest("#tn004");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn005) {
    performToRDFTestFromManifest("#tn005");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn006) {
    performToRDFTestFromManifest("#tn006");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn007) {
    performToRDFTestFromManifest("#tn007");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tn008) {
    performToRDFTestFromManifest("#tn008");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt01) {
    performToRDFTestFromManifest("#tnt01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt02) {
    performToRDFTestFromManifest("#tnt02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt03) {
    performToRDFTestFromManifest("#tnt03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt04) {
    performToRDFTestFromManifest("#tnt04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt05) {
    performToRDFTestFromManifest("#tnt05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt06) {
    performToRDFTestFromManifest("#tnt06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt07) {
    performToRDFTestFromManifest("#tnt07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt08) {
    performToRDFTestFromManifest("#tnt08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt09) {
    performToRDFTestFromManifest("#tnt09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt10) {
    performToRDFTestFromManifest("#tnt10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt11) {
    performToRDFTestFromManifest("#tnt11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt12) {
    performToRDFTestFromManifest("#tnt12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt13) {
    performToRDFTestFromManifest("#tnt13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt14) {
    performToRDFTestFromManifest("#tnt14");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt15) {
    performToRDFTestFromManifest("#tnt15");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tnt16) {
    performToRDFTestFromManifest("#tnt16");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tp001) {
    performToRDFTestFromManifest("#tp001");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tp002) {
    performToRDFTestFromManifest("#tp002");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tp003) {
    performToRDFTestFromManifest("#tp003");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tp004) {
    performToRDFTestFromManifest("#tp004");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi01) {
    performToRDFTestFromManifest("#tpi01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi02) {
    performToRDFTestFromManifest("#tpi02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi03) {
    performToRDFTestFromManifest("#tpi03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi04) {
    performToRDFTestFromManifest("#tpi04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi05) {
    performToRDFTestFromManifest("#tpi05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi06) {
    performToRDFTestFromManifest("#tpi06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi07) {
    performToRDFTestFromManifest("#tpi07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi08) {
    performToRDFTestFromManifest("#tpi08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi09) {
    performToRDFTestFromManifest("#tpi09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi10) {
    performToRDFTestFromManifest("#tpi10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpi11) {
    performToRDFTestFromManifest("#tpi11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr01) {
    performToRDFTestFromManifest("#tpr01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr02) {
    performToRDFTestFromManifest("#tpr02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr03) {
    performToRDFTestFromManifest("#tpr03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr04) {
    performToRDFTestFromManifest("#tpr04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr05) {
    performToRDFTestFromManifest("#tpr05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr06) {
    performToRDFTestFromManifest("#tpr06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr08) {
    performToRDFTestFromManifest("#tpr08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr09) {
    performToRDFTestFromManifest("#tpr09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr10) {
    performToRDFTestFromManifest("#tpr10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr11) {
    performToRDFTestFromManifest("#tpr11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr12) {
    performToRDFTestFromManifest("#tpr12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr13) {
    performToRDFTestFromManifest("#tpr13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr14) {
    performToRDFTestFromManifest("#tpr14");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr15) {
    performToRDFTestFromManifest("#tpr15");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr16) {
    performToRDFTestFromManifest("#tpr16");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr17) {
    performToRDFTestFromManifest("#tpr17");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr18) {
    performToRDFTestFromManifest("#tpr18");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr19) {
    performToRDFTestFromManifest("#tpr19");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr20) {
    performToRDFTestFromManifest("#tpr20");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr21) {
    performToRDFTestFromManifest("#tpr21");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr22) {
    performToRDFTestFromManifest("#tpr22");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr23) {
    performToRDFTestFromManifest("#tpr23");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr24) {
    performToRDFTestFromManifest("#tpr24");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr25) {
    performToRDFTestFromManifest("#tpr25");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr26) {
    performToRDFTestFromManifest("#tpr26");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr27) {
    performToRDFTestFromManifest("#tpr27");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr28) {
    performToRDFTestFromManifest("#tpr28");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr29) {
    performToRDFTestFromManifest("#tpr29");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr30) {
    performToRDFTestFromManifest("#tpr30");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr31) {
    performToRDFTestFromManifest("#tpr31");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr32) {
    performToRDFTestFromManifest("#tpr32");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr33) {
    performToRDFTestFromManifest("#tpr33");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr34) {
    performToRDFTestFromManifest("#tpr34");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr35) {
    performToRDFTestFromManifest("#tpr35");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr36) {
    performToRDFTestFromManifest("#tpr36");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr37) {
    performToRDFTestFromManifest("#tpr37");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr38) {
    performToRDFTestFromManifest("#tpr38");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr39) {
    performToRDFTestFromManifest("#tpr39");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tpr40) {
    performToRDFTestFromManifest("#tpr40");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_trt01) {
    performToRDFTestFromManifest("#trt01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso01) {
    performToRDFTestFromManifest("#tso01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso02) {
    performToRDFTestFromManifest("#tso02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso03) {
    performToRDFTestFromManifest("#tso03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso05) {
    performToRDFTestFromManifest("#tso05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso06) {
    performToRDFTestFromManifest("#tso06");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso07) {
    performToRDFTestFromManifest("#tso07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso08) {
    performToRDFTestFromManifest("#tso08");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso09) {
    performToRDFTestFromManifest("#tso09");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso10) {
    performToRDFTestFromManifest("#tso10");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso11) {
    performToRDFTestFromManifest("#tso11");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso12) {
    performToRDFTestFromManifest("#tso12");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_tso13) {
    performToRDFTestFromManifest("#tso13");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ttn01) {
    performToRDFTestFromManifest("#ttn01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_ttn02) {
    performToRDFTestFromManifest("#ttn02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf01) {
    performToRDFTestFromManifest("#twf01");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf02) {
    performToRDFTestFromManifest("#twf02");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf03) {
    performToRDFTestFromManifest("#twf03");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf04) {
    performToRDFTestFromManifest("#twf04");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf05) {
    performToRDFTestFromManifest("#twf05");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_twf07) {
    performToRDFTestFromManifest("#twf07");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performToRDFTestFromManifest("#t0300", "toRdf-manifest-extra.jsonld");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performToRDFTestFromManifest("#t0301", "toRdf-manifest-extra.jsonld");
}

TEST_F(JsonLdProcessorToRDFTest, toRdf_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performToRDFTestFromManifest("#t0302", "toRdf-manifest-extra.jsonld");
}

// This test would run everything in the manifest all at once, but is disabled for now
// until after we get everything working using the single test functions above.
TEST_F(JsonLdProcessorToRDFTest, DISABLED_toRdf_with_manifest) {

    ManifestLoader manifestLoader(resolvePath("test/testjsonld-cpp/test_data/"), "toRdf-manifest.jsonld");
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    for(auto & testCaseEntry : testCases) {
        std::cout << testCaseEntry.first << std::endl;

        auto testCase = testCaseEntry.second;

        performToRDFTest(testCase);
    }

}
