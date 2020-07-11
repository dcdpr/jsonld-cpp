#include "JsonLdProcessor.h"
#include "testHelpers.h"
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

void performToRDFTest(int testNumber) {

    std::string testName = "toRdf";
    std::string testNumberStr = getTestNumberStr(testNumber);

    std::string baseUri = getBaseUri(testName, testNumberStr);
    std::string inputStr = getInputStr(testName, testNumberStr);
    std::string expected = getExpectedRDF(testName, testNumberStr);

    std::unique_ptr<FileLoader> loader(new FileLoader);
    loader->addDocumentToCache(baseUri, inputStr);
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
    loader->addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(std::move(loader));

    std::string str = JsonLdProcessor::toRDFString(baseUri, opts);

    EXPECT_EQ(expected, str);
}

TEST(JsonLdProcessorTest, toRDF_0001) {
    performToRDFTest(1);
}

TEST(JsonLdProcessorTest, toRDF_0002) {
    performToRDFTest(2);
}

TEST(JsonLdProcessorTest, toRDF_0003) {
    performToRDFTest(3);
}

TEST(JsonLdProcessorTest, toRDF_0004) {
    performToRDFTest(4);
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRDF_0005) {
//    performToRDFTest(5);
//}

TEST(JsonLdProcessorTest, toRDF_0006) {
    performToRDFTest(6);
}

TEST(JsonLdProcessorTest, toRDF_0007) {
    performToRDFTest(7);
}

TEST(JsonLdProcessorTest, toRDF_0008) {
    performToRDFTest(8);
}

TEST(JsonLdProcessorTest, toRDF_0009) {
    performToRDFTest(9);
}

TEST(JsonLdProcessorTest, toRDF_0010) {
    performToRDFTest(10);
}

TEST(JsonLdProcessorTest, toRDF_0011) {
    performToRDFTest(11);
}

TEST(JsonLdProcessorTest, toRDF_0012) {
    performToRDFTest(12);
}

TEST(JsonLdProcessorTest, toRDF_0013) {
    performToRDFTest(13);
}

TEST(JsonLdProcessorTest, toRDF_0014) {
    performToRDFTest(14);
}

TEST(JsonLdProcessorTest, toRDF_0015) {
    performToRDFTest(15);
}

TEST(JsonLdProcessorTest, toRDF_0016) {
    performToRDFTest(16);
}

TEST(JsonLdProcessorTest, toRDF_0017) {
    performToRDFTest(17);
}

TEST(JsonLdProcessorTest, toRDF_0018) {
    performToRDFTest(18);
}

TEST(JsonLdProcessorTest, toRDF_0019) {
    performToRDFTest(19);
}

TEST(JsonLdProcessorTest, toRDF_0020) {
    performToRDFTest(20);
}

// case 21 doesn't exist

TEST(JsonLdProcessorTest, toRDF_0022) {
    performToRDFTest(22);
}

TEST(JsonLdProcessorTest, toRDF_0023) {
    performToRDFTest(23);
}

TEST(JsonLdProcessorTest, toRDF_0024) {
    performToRDFTest(24);
}

TEST(JsonLdProcessorTest, toRDF_0025) {
    performToRDFTest(25);
}

TEST(JsonLdProcessorTest, toRDF_0026) {
    performToRDFTest(26);
}

TEST(JsonLdProcessorTest, toRDF_0027) {
    performToRDFTest(27);
}

TEST(JsonLdProcessorTest, toRDF_0028) {
    performToRDFTest(28);
}

TEST(JsonLdProcessorTest, toRDF_0029) {
    performToRDFTest(29);
}

TEST(JsonLdProcessorTest, toRDF_0030) {
    performToRDFTest(30);
}

TEST(JsonLdProcessorTest, toRDF_0031) {
    performToRDFTest(31);
}

TEST(JsonLdProcessorTest, toRDF_0032) {
    performToRDFTest(32);
}

TEST(JsonLdProcessorTest, toRDF_0033) {
    performToRDFTest(33);
}

TEST(JsonLdProcessorTest, toRDF_0034) {
    performToRDFTest(34);
}

TEST(JsonLdProcessorTest, toRDF_0035) {
    performToRDFTest(35);
}

TEST(JsonLdProcessorTest, toRDF_0036) {
    performToRDFTest(36);
}

// cases 37-112 don't exist

TEST(JsonLdProcessorTest, toRdf_0113) {
    performToRDFTest(113);
}

TEST(JsonLdProcessorTest, toRdf_0114) {
    performToRDFTest(114);
}

TEST(JsonLdProcessorTest, toRdf_0115) {
    performToRDFTest(115);
}

TEST(JsonLdProcessorTest, toRdf_0116) {
    performToRDFTest(116);
}

TEST(JsonLdProcessorTest, toRdf_0117) {
    performToRDFTest(117);
}

//
//TEST(JsonLdProcessorTest, toRdf_0118) {
//    performToRDFTest(118);
//}

TEST(JsonLdProcessorTest, toRdf_0119) {
    performToRDFTest(119);
}

TEST(JsonLdProcessorTest, toRdf_e001) {
    performToRDFTest("e", 1);
}

TEST(JsonLdProcessorTest, toRdf_e002) {
    performToRDFTest("e", 2);
}

TEST(JsonLdProcessorTest, toRdf_e003) {
    performToRDFTest("e", 3);
}

TEST(JsonLdProcessorTest, toRdf_e004) {
    performToRDFTest("e", 4);
}

TEST(JsonLdProcessorTest, toRdf_e005) {
    performToRDFTest("e", 5);
}

TEST(JsonLdProcessorTest, toRdf_e006) {
    performToRDFTest("e", 6);
}

TEST(JsonLdProcessorTest, toRdf_e007) {
    performToRDFTest("e", 7);
}

TEST(JsonLdProcessorTest, toRdf_e008) {
    performToRDFTest("e", 8);
}

TEST(JsonLdProcessorTest, toRdf_e009) {
    performToRDFTest("e", 9);
}

TEST(JsonLdProcessorTest, toRdf_e010) {
    performToRDFTest("e", 10);
}

TEST(JsonLdProcessorTest, toRdf_e011) {
    performToRDFTest("e", 11);
}

TEST(JsonLdProcessorTest, toRdf_e012) {
    performToRDFTest("e", 12);
}

TEST(JsonLdProcessorTest, toRdf_e013) {
    performToRDFTest("e", 13);
}

TEST(JsonLdProcessorTest, toRdf_e014) {
    performToRDFTest("e", 14);
}

TEST(JsonLdProcessorTest, toRdf_e015) {
    performToRDFTest("e", 15);
}

TEST(JsonLdProcessorTest, toRdf_e016) {
    performToRDFTest("e", 16);
}

TEST(JsonLdProcessorTest, toRdf_e017) {
    performToRDFTest("e", 17);
}

TEST(JsonLdProcessorTest, toRdf_e018) {
    performToRDFTest("e", 18);
}

TEST(JsonLdProcessorTest, toRdf_e019) {
    performToRDFTest("e", 19);
}

TEST(JsonLdProcessorTest, toRdf_e020) {
    performToRDFTest("e", 20);
}

TEST(JsonLdProcessorTest, toRdf_e021) {
    performToRDFTest("e", 21);
}

TEST(JsonLdProcessorTest, toRdf_e022) {
    performToRDFTest("e", 22);
}

TEST(JsonLdProcessorTest, toRdf_e023) {
    performToRDFTest("e", 23);
}

TEST(JsonLdProcessorTest, toRdf_e024) {
    performToRDFTest("e", 24);
}

TEST(JsonLdProcessorTest, toRdf_e025) {
    performToRDFTest("e", 25);
}

TEST(JsonLdProcessorTest, toRdf_e026) {
    performToRDFTest("e", 26);
}

TEST(JsonLdProcessorTest, toRdf_e027) {
    performToRDFTest("e", 27);
}

TEST(JsonLdProcessorTest, toRdf_e028) {
    performToRDFTest("e", 28);
}

TEST(JsonLdProcessorTest, toRdf_e029) {
    performToRDFTest("e", 29);
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRdf_e030) {
//    performToRDFTest("e", 30);
//}

TEST(JsonLdProcessorTest, toRdf_e031) {
    performToRDFTest("e", 31);
}

TEST(JsonLdProcessorTest, toRdf_e032) {
    performToRDFTest("e", 32);
}

TEST(JsonLdProcessorTest, toRdf_e033) {
    performToRDFTest("e", 33);
}

TEST(JsonLdProcessorTest, toRdf_e034) {
    performToRDFTest("e", 34);
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRdf_e035) {
//    performToRDFTest("e", 35);
//}

TEST(JsonLdProcessorTest, toRdf_e036) {
    performToRDFTest("e", 36);
}

TEST(JsonLdProcessorTest, toRdf_e037) {
    performToRDFTest("e", 37);
}

TEST(JsonLdProcessorTest, toRdf_e038) {
    performToRDFTest("e", 38);
}

TEST(JsonLdProcessorTest, toRdf_e039) {
    performToRDFTest("e", 39);
}

TEST(JsonLdProcessorTest, toRdf_e040) {
    performToRDFTest("e", 40);
}

TEST(JsonLdProcessorTest, toRdf_e041) {
    performToRDFTest("e", 41);
}

TEST(JsonLdProcessorTest, toRdf_e042) {
    performToRDFTest("e", 42);
}

TEST(JsonLdProcessorTest, toRdf_e043) {
    performToRDFTest("e", 43);
}

TEST(JsonLdProcessorTest, toRdf_e044) {
    performToRDFTest("e", 44);
}

TEST(JsonLdProcessorTest, toRdf_e045) {
    performToRDFTest("e", 45);
}

TEST(JsonLdProcessorTest, toRdf_e046) {
    performToRDFTest("e", 46);
}

TEST(JsonLdProcessorTest, toRdf_e047) {
    performToRDFTest("e", 47);
}

TEST(JsonLdProcessorTest, toRdf_e048) {
    performToRDFTest("e", 48);
}

TEST(JsonLdProcessorTest, toRdf_e049) {
    performToRDFTest("e", 49);
}

TEST(JsonLdProcessorTest, toRdf_e050) {
    performToRDFTest("e", 50);
}

TEST(JsonLdProcessorTest, toRdf_e051) {
    performToRDFTest("e", 51);
}

TEST(JsonLdProcessorTest, toRdf_e052) {
    performToRDFTest("e", 52);
}

TEST(JsonLdProcessorTest, toRdf_e053) {
    performToRDFTest("e", 53);
}

TEST(JsonLdProcessorTest, toRdf_e054) {
    performToRDFTest("e", 54);
}

TEST(JsonLdProcessorTest, toRdf_e055) {
    performToRDFTest("e", 55);
}

TEST(JsonLdProcessorTest, toRdf_e056) {
    performToRDFTest("e", 56);
}

TEST(JsonLdProcessorTest, toRdf_e057) {
    performToRDFTest("e", 57);
}

TEST(JsonLdProcessorTest, toRdf_e058) {
    performToRDFTest("e", 58);
}

TEST(JsonLdProcessorTest, toRdf_e059) {
    performToRDFTest("e", 59);
}

TEST(JsonLdProcessorTest, toRdf_e060) {
    performToRDFTest("e", 60);
}

TEST(JsonLdProcessorTest, toRdf_e061) {
    performToRDFTest("e", 61);
}

TEST(JsonLdProcessorTest, toRdf_e062) {
    performToRDFTest("e", 62);
}

TEST(JsonLdProcessorTest, toRdf_e063) {
    performToRDFTest("e", 63);
}

TEST(JsonLdProcessorTest, toRdf_e064) {
    performToRDFTest("e", 64);
}

TEST(JsonLdProcessorTest, toRdf_e065) {
    performToRDFTest("e", 65);
}

TEST(JsonLdProcessorTest, toRdf_e066) {
    performToRDFTest("e", 66);
}

TEST(JsonLdProcessorTest, toRdf_e067) {
    performToRDFTest("e", 67);
}

TEST(JsonLdProcessorTest, toRdf_e068) {
    performToRDFTest("e", 68);
}

TEST(JsonLdProcessorTest, toRdf_e069) {
    performToRDFTest("e", 69);
}

TEST(JsonLdProcessorTest, toRdf_e070) {
    performToRDFTest("e", 70);
}

TEST(JsonLdProcessorTest, toRdf_e071) {
    performToRDFTest("e", 71);
}

TEST(JsonLdProcessorTest, toRdf_e072) {
    performToRDFTest("e", 72);
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
//                performToRDFTest(i);
//        }
//    }
//}

TEST(JsonLdProcessorTest, toRdf_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performToRDFTest(300);
}

TEST(JsonLdProcessorTest, toRdf_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performToRDFTest(301);
}

TEST(JsonLdProcessorTest, toRdf_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performToRDFTest(302);
}

