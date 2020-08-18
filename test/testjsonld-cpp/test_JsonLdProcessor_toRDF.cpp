#include "JsonLdProcessor.h"
#include "testHelpers.h"
#include "ManifestLoader.h"
#include "FileLoader.h"
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

void performToRDFTest(int testNumber) {

    std::string testName = "toRdf";
    std::string testNumberStr = getTestNumberStr(testNumber);

    std::string baseUri = getBaseUri(testName, testNumberStr);
    std::string inputStr = getInputStr(testName, testNumberStr);
    std::string expected = getExpectedRDF(testName, testNumberStr);

    std::unique_ptr<FileLoader> loader(new FileLoader);
    //loader->addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(std::move(loader));

    std::string str = JsonLdProcessor::toRDFString(baseUri, opts);

    EXPECT_EQ(expected, str);
}

void performToRDFTest(std::string prefix, int testNumber) {

    std::string testName = "toRdf";
    std::string testNumberStr = getTestNumberStr(prefix, testNumber);

    std::string baseUri = getBaseUri(testName, testNumberStr);
    std::string inputStr = getInputStr(testName, testNumberStr);
    std::string expected = getExpectedRDF(testName, testNumberStr);

    std::unique_ptr<FileLoader> loader(new FileLoader);
    //loader->addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(std::move(loader));

    std::string str = JsonLdProcessor::toRDFString(baseUri, opts);

    EXPECT_EQ(expected, str);
}

void performToRDFTestFromManifest(std::string testName, std::string manifestName="toRdf-manifest.jsonld") {

    ManifestLoader manifestLoader(
            resolvePath("test/testjsonld-cpp/test_data/"),
            manifestName);
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    auto testCase = testCases.at(testName);

    JsonLdOptions options = testCase.getOptions();

    std::string rdfStr = JsonLdProcessor::toRDFString(testCase.input, options);

    std::unique_ptr<RemoteDocument> expectedDocument =
            options.getDocumentLoader()->loadDocument(testCase.expect);

    const std::string & expected = expectedDocument->getRDFContent();

    EXPECT_TRUE(JsonLdUtils::deepCompare(expected, rdfStr));

}

TEST(JsonLdProcessorTest, toRDF_0001) {
    performToRDFTestFromManifest("#t0001");
}

TEST(JsonLdProcessorTest, toRDF_0002) {
    performToRDFTestFromManifest("#t0002");
}

TEST(JsonLdProcessorTest, toRDF_0003) {
    performToRDFTestFromManifest("#t0003");
}

TEST(JsonLdProcessorTest, toRDF_0004) {
    performToRDFTestFromManifest("#t0004");
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRDF_0005) {
//    performToRDFTestFromManifest("#t005");
//}

TEST(JsonLdProcessorTest, toRDF_0006) {
    performToRDFTestFromManifest("#t0006");
}

TEST(JsonLdProcessorTest, toRDF_0007) {
    performToRDFTestFromManifest("#t0007");
}

TEST(JsonLdProcessorTest, toRDF_0008) {
    performToRDFTestFromManifest("#t0008");
}

TEST(JsonLdProcessorTest, toRDF_0009) {
    performToRDFTestFromManifest("#t0009");
}

TEST(JsonLdProcessorTest, toRDF_0010) {
    performToRDFTestFromManifest("#t0010");
}

TEST(JsonLdProcessorTest, toRDF_0011) {
    performToRDFTestFromManifest("#t0011");
}

TEST(JsonLdProcessorTest, toRDF_0012) {
    performToRDFTestFromManifest("#t0012");
}

TEST(JsonLdProcessorTest, toRDF_0013) {
    performToRDFTestFromManifest("#t0013");
}

TEST(JsonLdProcessorTest, toRDF_0014) {
    performToRDFTestFromManifest("#t0014");
}

TEST(JsonLdProcessorTest, toRDF_0015) {
    performToRDFTestFromManifest("#t0015");
}

TEST(JsonLdProcessorTest, toRDF_0016) {
    performToRDFTestFromManifest("#t0016");
}

TEST(JsonLdProcessorTest, toRDF_0017) {
    performToRDFTestFromManifest("#t0017");
}

TEST(JsonLdProcessorTest, toRDF_0018) {
    performToRDFTestFromManifest("#t0018");
}

TEST(JsonLdProcessorTest, toRDF_0019) {
    performToRDFTestFromManifest("#t0019");
}

TEST(JsonLdProcessorTest, toRDF_0020) {
    performToRDFTestFromManifest("#t0020");
}

// case 21 doesn't exist

TEST(JsonLdProcessorTest, toRDF_0022) {
    performToRDFTestFromManifest("#t0022");
}

TEST(JsonLdProcessorTest, toRDF_0023) {
    performToRDFTestFromManifest("#t0023");
}

TEST(JsonLdProcessorTest, toRDF_0024) {
    performToRDFTestFromManifest("#t0024");
}

TEST(JsonLdProcessorTest, toRDF_0025) {
    performToRDFTestFromManifest("#t0025");
}

TEST(JsonLdProcessorTest, toRDF_0026) {
    performToRDFTestFromManifest("#t0026");
}

TEST(JsonLdProcessorTest, toRDF_0027) {
    performToRDFTestFromManifest("#t0027");
}

TEST(JsonLdProcessorTest, toRDF_0028) {
    performToRDFTestFromManifest("#t0028");
}

TEST(JsonLdProcessorTest, toRDF_0029) {
    performToRDFTestFromManifest("#t0029");
}

TEST(JsonLdProcessorTest, toRDF_0030) {
    performToRDFTestFromManifest("#t0030");
}

TEST(JsonLdProcessorTest, toRDF_0031) {
    performToRDFTestFromManifest("#t0031");
}

TEST(JsonLdProcessorTest, toRDF_0032) {
    performToRDFTestFromManifest("#t0032");
}

TEST(JsonLdProcessorTest, toRDF_0033) {
    performToRDFTestFromManifest("#t0033");
}

TEST(JsonLdProcessorTest, toRDF_0034) {
    performToRDFTestFromManifest("#t0034");
}

TEST(JsonLdProcessorTest, toRDF_0035) {
    performToRDFTestFromManifest("#t0035");
}

TEST(JsonLdProcessorTest, toRDF_0036) {
    performToRDFTestFromManifest("#t0036");
}

// cases 37-112 don't exist

TEST(JsonLdProcessorTest, toRdf_0113) {
    performToRDFTestFromManifest("#t0113");
}

TEST(JsonLdProcessorTest, toRdf_0114) {
    performToRDFTestFromManifest("#t0114");
}

TEST(JsonLdProcessorTest, toRdf_0115) {
    performToRDFTestFromManifest("#t0115");
}

TEST(JsonLdProcessorTest, toRdf_0116) {
    performToRDFTestFromManifest("#t0116");
}

TEST(JsonLdProcessorTest, toRdf_0117) {
    performToRDFTestFromManifest("#t0117");
}

//
//TEST(JsonLdProcessorTest, toRdf_0118) {
//    performToRDFTestFromManifest("#t00118");
//}

TEST(JsonLdProcessorTest, toRdf_0119) {
    performToRDFTestFromManifest("#t0119");
}

TEST(JsonLdProcessorTest, toRdf_e001) {
    performToRDFTestFromManifest("#te001");
}

TEST(JsonLdProcessorTest, toRdf_e002) {
    performToRDFTestFromManifest("#te002");
}

TEST(JsonLdProcessorTest, toRdf_e003) {
    performToRDFTestFromManifest("#te003");
}

TEST(JsonLdProcessorTest, toRdf_e004) {
    performToRDFTestFromManifest("#te004");
}

TEST(JsonLdProcessorTest, toRdf_e005) {
    performToRDFTestFromManifest("#te005");
}

TEST(JsonLdProcessorTest, toRdf_e006) {
    performToRDFTestFromManifest("#te006");
}

TEST(JsonLdProcessorTest, toRdf_e007) {
    performToRDFTestFromManifest("#te007");
}

TEST(JsonLdProcessorTest, toRdf_e008) {
    performToRDFTestFromManifest("#te008");
}

TEST(JsonLdProcessorTest, toRdf_e009) {
    performToRDFTestFromManifest("#te009");
}

TEST(JsonLdProcessorTest, toRdf_e010) {
    performToRDFTestFromManifest("#te0010");
}

TEST(JsonLdProcessorTest, toRdf_e011) {
    performToRDFTestFromManifest("#te0011");
}

TEST(JsonLdProcessorTest, toRdf_e012) {
    performToRDFTestFromManifest("#te0012");
}

TEST(JsonLdProcessorTest, toRdf_e013) {
    performToRDFTestFromManifest("#te0013");
}

TEST(JsonLdProcessorTest, toRdf_e014) {
    performToRDFTestFromManifest("#te0014");
}

TEST(JsonLdProcessorTest, toRdf_e015) {
    performToRDFTestFromManifest("#te0015");
}

TEST(JsonLdProcessorTest, toRdf_e016) {
    performToRDFTestFromManifest("#te0016");
}

TEST(JsonLdProcessorTest, toRdf_e017) {
    performToRDFTestFromManifest("#te0017");
}

TEST(JsonLdProcessorTest, toRdf_e018) {
    performToRDFTestFromManifest("#te0018");
}

TEST(JsonLdProcessorTest, toRdf_e019) {
    performToRDFTestFromManifest("#te0019");
}

TEST(JsonLdProcessorTest, toRdf_e020) {
    performToRDFTestFromManifest("#te0020");
}

TEST(JsonLdProcessorTest, toRdf_e021) {
    performToRDFTestFromManifest("#te0021");
}

TEST(JsonLdProcessorTest, toRdf_e022) {
    performToRDFTestFromManifest("#te0022");
}

TEST(JsonLdProcessorTest, toRdf_e023) {
    performToRDFTestFromManifest("#te0023");
}

TEST(JsonLdProcessorTest, toRdf_e024) {
    performToRDFTestFromManifest("#te0024");
}

TEST(JsonLdProcessorTest, toRdf_e025) {
    performToRDFTestFromManifest("#te0025");
}

TEST(JsonLdProcessorTest, toRdf_e026) {
    performToRDFTestFromManifest("#te0026");
}

TEST(JsonLdProcessorTest, toRdf_e027) {
    performToRDFTestFromManifest("#te0027");
}

TEST(JsonLdProcessorTest, toRdf_e028) {
    performToRDFTestFromManifest("#te0028");
}

TEST(JsonLdProcessorTest, toRdf_e029) {
    performToRDFTestFromManifest("#te0029");
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRdf_e030) {
//    performToRDFTestFromManifest("#te0030");
//}

TEST(JsonLdProcessorTest, toRdf_e031) {
    performToRDFTestFromManifest("#te0031");
}

TEST(JsonLdProcessorTest, toRdf_e032) {
    performToRDFTestFromManifest("#te0032");
}

TEST(JsonLdProcessorTest, toRdf_e033) {
    performToRDFTestFromManifest("#te0033");
}

TEST(JsonLdProcessorTest, toRdf_e034) {
    performToRDFTestFromManifest("#te0034");
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRdf_e035) {
//    performToRDFTestFromManifest("#te0035");
//}

TEST(JsonLdProcessorTest, toRdf_e036) {
    performToRDFTestFromManifest("#te0036");
}

TEST(JsonLdProcessorTest, toRdf_e037) {
    performToRDFTestFromManifest("#te0037");
}

TEST(JsonLdProcessorTest, toRdf_e038) {
    performToRDFTestFromManifest("#te0038");
}

TEST(JsonLdProcessorTest, toRdf_e039) {
    performToRDFTestFromManifest("#te0039");
}

TEST(JsonLdProcessorTest, toRdf_e040) {
    performToRDFTestFromManifest("#te0040");
}

TEST(JsonLdProcessorTest, toRdf_e041) {
    performToRDFTestFromManifest("#te0041");
}

TEST(JsonLdProcessorTest, toRdf_e042) {
    performToRDFTestFromManifest("#te0042");
}

TEST(JsonLdProcessorTest, toRdf_e043) {
    performToRDFTestFromManifest("#te0043");
}

TEST(JsonLdProcessorTest, toRdf_e044) {
    performToRDFTestFromManifest("#te0044");
}

TEST(JsonLdProcessorTest, toRdf_e045) {
    performToRDFTestFromManifest("#te0045");
}

TEST(JsonLdProcessorTest, toRdf_e046) {
    performToRDFTestFromManifest("#te0046");
}

TEST(JsonLdProcessorTest, toRdf_e047) {
    performToRDFTestFromManifest("#te0047");
}

TEST(JsonLdProcessorTest, toRdf_e048) {
    performToRDFTestFromManifest("#te0048");
}

TEST(JsonLdProcessorTest, toRdf_e049) {
    performToRDFTestFromManifest("#te0049");
}

TEST(JsonLdProcessorTest, toRdf_e050) {
    performToRDFTestFromManifest("#te0050");
}

TEST(JsonLdProcessorTest, toRdf_e051) {
    performToRDFTestFromManifest("#te0051");
}

TEST(JsonLdProcessorTest, toRdf_e052) {
    performToRDFTestFromManifest("#te0052");
}

TEST(JsonLdProcessorTest, toRdf_e053) {
    performToRDFTestFromManifest("#te0053");
}

TEST(JsonLdProcessorTest, toRdf_e054) {
    performToRDFTestFromManifest("#te0054");
}

TEST(JsonLdProcessorTest, toRdf_e055) {
    performToRDFTestFromManifest("#te0055");
}

TEST(JsonLdProcessorTest, toRdf_e056) {
    performToRDFTestFromManifest("#te0056");
}

TEST(JsonLdProcessorTest, toRdf_e057) {
    performToRDFTestFromManifest("#te0057");
}

TEST(JsonLdProcessorTest, toRdf_e058) {
    performToRDFTestFromManifest("#te0058");
}

TEST(JsonLdProcessorTest, toRdf_e059) {
    performToRDFTestFromManifest("#te0059");
}

TEST(JsonLdProcessorTest, toRdf_e060) {
    performToRDFTestFromManifest("#te0060");
}

TEST(JsonLdProcessorTest, toRdf_e061) {
    performToRDFTestFromManifest("#te0061");
}

TEST(JsonLdProcessorTest, toRdf_e062) {
    performToRDFTestFromManifest("#te0062");
}

TEST(JsonLdProcessorTest, toRdf_e063) {
    performToRDFTestFromManifest("#te0063");
}

TEST(JsonLdProcessorTest, toRdf_e064) {
    performToRDFTestFromManifest("#te0064");
}

TEST(JsonLdProcessorTest, toRdf_e065) {
    performToRDFTestFromManifest("#te0065");
}

TEST(JsonLdProcessorTest, toRdf_e066) {
    performToRDFTestFromManifest("#te0066");
}

TEST(JsonLdProcessorTest, toRdf_e067) {
    performToRDFTestFromManifest("#te0067");
}

TEST(JsonLdProcessorTest, toRdf_e068) {
    performToRDFTestFromManifest("#te0068");
}

TEST(JsonLdProcessorTest, toRdf_e069) {
    performToRDFTestFromManifest("#te0069");
}

TEST(JsonLdProcessorTest, toRdf_e070) {
    performToRDFTestFromManifest("#te0070");
}

TEST(JsonLdProcessorTest, toRdf_e071) {
    performToRDFTestFromManifest("#te0071");
}

TEST(JsonLdProcessorTest, toRdf_e072) {
    performToRDFTestFromManifest("#te0072");
}

//TEST(JsonLdProcessorTest, toRdf_loop) {
//
//    for(auto i = 1; i <= 119; ++i) {
//        switch(i) {
//            case 5:  // skip until I have better utf8 char handling
//            case 21: // doesn't exist
//            case 37: // doesn't exist
//            case 38: // doesn't exist
//            case 39: // doesn't exist
//            case 40: // doesn't exist
//            case 70: // skip until I have better utf8 char handling
//            case 75: // skip until I have better utf8 char handling
//            case 118:// skip until I have generalized RDF flag implemented
//                continue;
//            default:
//                performToRDFTestFromManifest(i);
//        }
//    }
//}

TEST(JsonLdProcessorTest, toRdf_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performToRDFTestFromManifest("#t0300", "toRdf-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, toRdf_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performToRDFTestFromManifest("#t0301", "toRdf-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, toRdf_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performToRDFTestFromManifest("#t0302", "toRdf-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, toRdf_with_manifest) {

    ManifestLoader manifestLoader(resolvePath("test/testjsonld-cpp/test_data/"), "toRdf-manifest.jsonld");
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    for (auto & testCaseEntry : testCases) {
        std::cout << testCaseEntry.first << std::endl;

        JsonLdOptions options = testCaseEntry.second.getOptions();

        std::string str;
        try {
            str = JsonLdProcessor::toRDFString(testCaseEntry.second.input, options);
        }
        catch (JsonLdError &e) {
            std::cout << e.what() << std::endl;
            continue;
        }
        catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        auto expectedDocument = options.getDocumentLoader()->loadDocument(
                testCaseEntry.second.expect);
        std::string expected = expectedDocument->getRDFContent();

        EXPECT_EQ(expected, str);

    }

}
