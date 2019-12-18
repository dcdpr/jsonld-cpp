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

// cases 37-40 don't exist

TEST(JsonLdProcessorTest, toRDF_0041) {
    performToRDFTest(41);
}

TEST(JsonLdProcessorTest, toRDF_0042) {
    performToRDFTest(42);
}

TEST(JsonLdProcessorTest, toRDF_0043) {
    performToRDFTest(43);
}

TEST(JsonLdProcessorTest, toRDF_0044) {
    performToRDFTest(44);
}

TEST(JsonLdProcessorTest, toRDF_0045) {
    performToRDFTest(45);
}

TEST(JsonLdProcessorTest, toRDF_0046) {
    performToRDFTest(46);
}

TEST(JsonLdProcessorTest, toRDF_0047) {
    performToRDFTest(47);
}

TEST(JsonLdProcessorTest, toRDF_0048) {
    performToRDFTest(48);
}

TEST(JsonLdProcessorTest, toRDF_0049) {
    performToRDFTest(49);
}

TEST(JsonLdProcessorTest, toRDF_0050) {
    performToRDFTest(50);
}

TEST(JsonLdProcessorTest, toRDF_0051) {
    performToRDFTest(51);
}

TEST(JsonLdProcessorTest, toRDF_0052) {
    performToRDFTest(52);
}

TEST(JsonLdProcessorTest, toRDF_0053) {
    performToRDFTest(53);
}

TEST(JsonLdProcessorTest, toRDF_0054) {
    performToRDFTest(54);
}

TEST(JsonLdProcessorTest, toRDF_0055) {
    performToRDFTest(55);
}

TEST(JsonLdProcessorTest, toRDF_0056) {
    performToRDFTest(56);
}

TEST(JsonLdProcessorTest, toRDF_0057) {
    performToRDFTest(57);
}

TEST(JsonLdProcessorTest, toRDF_0058) {
    performToRDFTest(58);
}

TEST(JsonLdProcessorTest, toRDF_0059) {
    performToRDFTest(59);
}

TEST(JsonLdProcessorTest, toRDF_0060) {
    performToRDFTest(60);
}

TEST(JsonLdProcessorTest, toRDF_0061) {
    performToRDFTest(61);
}

TEST(JsonLdProcessorTest, toRDF_0062) {
    performToRDFTest(62);
}

TEST(JsonLdProcessorTest, toRDF_0063) {
    performToRDFTest(63);
}

TEST(JsonLdProcessorTest, toRDF_0064) {
    performToRDFTest(64);
}

TEST(JsonLdProcessorTest, toRDF_0065) {
    performToRDFTest(65);
}

TEST(JsonLdProcessorTest, toRDF_0066) {
    performToRDFTest(66);
}

TEST(JsonLdProcessorTest, toRDF_0067) {
    performToRDFTest(67);
}

TEST(JsonLdProcessorTest, toRDF_0068) {
    performToRDFTest(68);
}

TEST(JsonLdProcessorTest, toRDF_0069) {
    performToRDFTest(69);
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRDF_0070) {
//    performToRDFTest(70);
//}

TEST(JsonLdProcessorTest, toRDF_0071) {
    performToRDFTest(71);
}

TEST(JsonLdProcessorTest, toRDF_0072) {
    performToRDFTest(72);
}

TEST(JsonLdProcessorTest, toRDF_0073) {
    performToRDFTest(73);
}

TEST(JsonLdProcessorTest, toRDF_0074) {
    performToRDFTest(74);
}

// skip until I have better utf8 char handling
//TEST(JsonLdProcessorTest, toRDF_0075) {
//    performToRDFTest(75);
//}

TEST(JsonLdProcessorTest, toRdf_0076) {
    performToRDFTest(76);
}

TEST(JsonLdProcessorTest, toRdf_0077) {
    performToRDFTest(77);
}

TEST(JsonLdProcessorTest, toRdf_0078) {
    performToRDFTest(78);
}

TEST(JsonLdProcessorTest, toRdf_0079) {
    performToRDFTest(79);
}

TEST(JsonLdProcessorTest, toRdf_0080) {
    performToRDFTest(80);
}

TEST(JsonLdProcessorTest, toRdf_0081) {
    performToRDFTest(81);
}

TEST(JsonLdProcessorTest, toRdf_0082) {
    performToRDFTest(82);
}

TEST(JsonLdProcessorTest, toRdf_0083) {
    performToRDFTest(83);
}

TEST(JsonLdProcessorTest, toRdf_0084) {
    performToRDFTest(84);
}

TEST(JsonLdProcessorTest, toRdf_0085) {
    performToRDFTest(85);
}

TEST(JsonLdProcessorTest, toRdf_0086) {
    performToRDFTest(86);
}

TEST(JsonLdProcessorTest, toRdf_0087) {
    performToRDFTest(87);
}

TEST(JsonLdProcessorTest, toRdf_0088) {
    performToRDFTest(88);
}

TEST(JsonLdProcessorTest, toRdf_0089) {
    performToRDFTest(89);
}

TEST(JsonLdProcessorTest, toRdf_0090) {
    performToRDFTest(90);
}

TEST(JsonLdProcessorTest, toRdf_0091) {
    performToRDFTest(91);
}

TEST(JsonLdProcessorTest, toRdf_0092) {
    performToRDFTest(92);
}

TEST(JsonLdProcessorTest, toRdf_0093) {
    performToRDFTest(93);
}

TEST(JsonLdProcessorTest, toRdf_0094) {
    performToRDFTest(94);
}

TEST(JsonLdProcessorTest, toRdf_0095) {
    performToRDFTest(95);
}

TEST(JsonLdProcessorTest, toRdf_0096) {
    performToRDFTest(96);
}

TEST(JsonLdProcessorTest, toRdf_0097) {
    performToRDFTest(97);
}

TEST(JsonLdProcessorTest, toRdf_0098) {
    performToRDFTest(98);
}

TEST(JsonLdProcessorTest, toRdf_0099) {
    performToRDFTest(99);
}

TEST(JsonLdProcessorTest, toRdf_0100) {
    performToRDFTest(100);
}

TEST(JsonLdProcessorTest, toRdf_0101) {
    performToRDFTest(101);
}

TEST(JsonLdProcessorTest, toRdf_0102) {
    performToRDFTest(102);
}

TEST(JsonLdProcessorTest, toRdf_0103) {
    performToRDFTest(103);
}

TEST(JsonLdProcessorTest, toRdf_0104) {
    performToRDFTest(104);
}

TEST(JsonLdProcessorTest, toRdf_0105) {
    performToRDFTest(105);
}

TEST(JsonLdProcessorTest, toRdf_0106) {
    performToRDFTest(106);
}

TEST(JsonLdProcessorTest, toRdf_0107) {
    performToRDFTest(107);
}

TEST(JsonLdProcessorTest, toRdf_0108) {
    performToRDFTest(108);
}

TEST(JsonLdProcessorTest, toRdf_0109) {
    performToRDFTest(109);
}

TEST(JsonLdProcessorTest, toRdf_0110) {
    performToRDFTest(110);
}

TEST(JsonLdProcessorTest, toRdf_0111) {
    performToRDFTest(111);
}

TEST(JsonLdProcessorTest, toRdf_0112) {
    performToRDFTest(112);
}

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

