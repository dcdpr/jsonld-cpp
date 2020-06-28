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

    DocumentLoader dl;
    dl.addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(dl);

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

