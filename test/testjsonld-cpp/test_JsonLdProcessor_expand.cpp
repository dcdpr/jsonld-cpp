#include "JsonLdProcessor.cpp"
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

void performExpandTest(int testNumber) {

    std::string testName = "expand";
    std::string testNumberStr = getTestNumberStr(testNumber);

    std::string baseUri = getBaseUri(testName, testNumberStr);
    std::string inputStr = getInputStr(testName, testNumberStr);
    json expected = getExpectedJson(testName, testNumberStr);

    DocumentLoader dl;
    dl.addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(dl);

    json expanded = JsonLdProcessor::expand(baseUri, opts);
std::cout << expanded << std::endl;
    EXPECT_TRUE(JsonLdUtils::deepCompare(expected, expanded));
}

TEST(JsonLdProcessorTest, expand_0001) {
    performExpandTest(1);
}

TEST(JsonLdProcessorTest, expand_0002) {
    performExpandTest(2);
}

TEST(JsonLdProcessorTest, expand_0003) {
    performExpandTest(3);
}

TEST(JsonLdProcessorTest, expand_0004) {
    performExpandTest(4);
}

TEST(JsonLdProcessorTest, expand_0005) {
    performExpandTest(5);
}

TEST(JsonLdProcessorTest, expand_0006) {
    performExpandTest(6);
}

TEST(JsonLdProcessorTest, expand_0007) {
    performExpandTest(7);
}

TEST(JsonLdProcessorTest, expand_0008) {
    performExpandTest(8);
}

TEST(JsonLdProcessorTest, expand_0009) {
    performExpandTest(9);
}

TEST(JsonLdProcessorTest, expand_0010) {
    performExpandTest(10);
}

TEST(JsonLdProcessorTest, expand_0011) {
    performExpandTest(11);
}

TEST(JsonLdProcessorTest, expand_0012) {
    performExpandTest(12);
}

TEST(JsonLdProcessorTest, expand_0013) {
    performExpandTest(13);
}

TEST(JsonLdProcessorTest, expand_0014) {
    performExpandTest(14);
}

TEST(JsonLdProcessorTest, expand_0015) {
    performExpandTest(15);
}

TEST(JsonLdProcessorTest, expand_0016) {
    performExpandTest(16);
}

TEST(JsonLdProcessorTest, expand_0017) {
    performExpandTest(17);
}

TEST(JsonLdProcessorTest, expand_0018) {
    performExpandTest(18);
}

TEST(JsonLdProcessorTest, expand_0019) {
    performExpandTest(19);
}

TEST(JsonLdProcessorTest, expand_0020) {
    performExpandTest(20);
}

TEST(JsonLdProcessorTest, expand_0021) {
    performExpandTest(21);
}

TEST(JsonLdProcessorTest, expand_0022) {
    performExpandTest(22);
}

TEST(JsonLdProcessorTest, expand_0023) {
    performExpandTest(23);
}

TEST(JsonLdProcessorTest, expand_0024) {
    performExpandTest(24);
}

TEST(JsonLdProcessorTest, expand_0025) {
    performExpandTest(25);
}

TEST(JsonLdProcessorTest, expand_0026) {
    performExpandTest(26);
}

TEST(JsonLdProcessorTest, expand_0027) {
    performExpandTest(27);
}

TEST(JsonLdProcessorTest, expand_0028) {
    performExpandTest(28);
}

TEST(JsonLdProcessorTest, expand_0029) {
    performExpandTest(29);
}

TEST(JsonLdProcessorTest, expand_0030) {
    performExpandTest(30);
}

TEST(JsonLdProcessorTest, expand_0031) {
    performExpandTest(31);
}

TEST(JsonLdProcessorTest, expand_0032) {
    performExpandTest(32);
}

TEST(JsonLdProcessorTest, expand_0033) {
    performExpandTest(33);
}

TEST(JsonLdProcessorTest, expand_0034) {
    performExpandTest(34);
}

TEST(JsonLdProcessorTest, expand_0035) {
    performExpandTest(35);
}

TEST(JsonLdProcessorTest, expand_0036) {
    performExpandTest(36);
}

TEST(JsonLdProcessorTest, expand_0037) {
    performExpandTest(37);
}

TEST(JsonLdProcessorTest, expand_0038) {
    performExpandTest(38);
}

TEST(JsonLdProcessorTest, expand_0039) {
    performExpandTest(39);
}

TEST(JsonLdProcessorTest, expand_0040) {
    performExpandTest(40);
}

TEST(JsonLdProcessorTest, expand_0041) {
    performExpandTest(41);
}

TEST(JsonLdProcessorTest, expand_0042) {
    performExpandTest(42);
}

TEST(JsonLdProcessorTest, expand_0043) {
    performExpandTest(43);
}

TEST(JsonLdProcessorTest, expand_0044) {
    performExpandTest(44);
}

TEST(JsonLdProcessorTest, expand_0045) {
    performExpandTest(45);
}

TEST(JsonLdProcessorTest, expand_0046) {
    performExpandTest(46);
}

TEST(JsonLdProcessorTest, expand_0047) {
    performExpandTest(47);
}

TEST(JsonLdProcessorTest, expand_0048) {
    performExpandTest(48);
}

TEST(JsonLdProcessorTest, expand_0049) {
    performExpandTest(49);
}

TEST(JsonLdProcessorTest, expand_0050) {
    performExpandTest(50);
}

TEST(JsonLdProcessorTest, expand_0051) {
    performExpandTest(51);
}

TEST(JsonLdProcessorTest, expand_0052) {
    performExpandTest(52);
}

TEST(JsonLdProcessorTest, expand_0053) {
    performExpandTest(53);
}

TEST(JsonLdProcessorTest, expand_0054) {
    performExpandTest(54);
}

TEST(JsonLdProcessorTest, expand_0055) {
    performExpandTest(55);
}

TEST(JsonLdProcessorTest, expand_0056) {
    performExpandTest(56);
}

TEST(JsonLdProcessorTest, expand_0057) {
    performExpandTest(57);
}

TEST(JsonLdProcessorTest, expand_0058) {
    performExpandTest(58);
}

TEST(JsonLdProcessorTest, expand_0059) {
    performExpandTest(59);
}

TEST(JsonLdProcessorTest, expand_0060) {
    performExpandTest(60);
}

TEST(JsonLdProcessorTest, expand_0061) {
    performExpandTest(61);
}

TEST(JsonLdProcessorTest, expand_0062) {
    performExpandTest(62);
}

TEST(JsonLdProcessorTest, expand_0063) {
    performExpandTest(63);
}

TEST(JsonLdProcessorTest, expand_0064) {
    performExpandTest(64);
}

TEST(JsonLdProcessorTest, expand_0065) {
    performExpandTest(65);
}

TEST(JsonLdProcessorTest, expand_0066) {
    performExpandTest(66);
}

TEST(JsonLdProcessorTest, expand_0067) {
    performExpandTest(67);
}

TEST(JsonLdProcessorTest, expand_0068) {
    performExpandTest(68);
}

TEST(JsonLdProcessorTest, expand_0069) {
    performExpandTest(69);
}

TEST(JsonLdProcessorTest, expand_0070) {
    performExpandTest(70);
}

TEST(JsonLdProcessorTest, expand_0071) {
    performExpandTest(71);
}

TEST(JsonLdProcessorTest, expand_0072) {
    performExpandTest(72);
}

TEST(JsonLdProcessorTest, expand_0073) {
    performExpandTest(73);
}

TEST(JsonLdProcessorTest, expand_0074) {
    performExpandTest(74);
}

TEST(JsonLdProcessorTest, expand_0075) {
    performExpandTest(75);
}

// Disabled this test because we need to implement the standard testing scaffold to pass in an
// extra @base option at the last minute
//TEST(JsonLdProcessorTest, expand_0076) {
//    performExpandTest(76);
//}

// Disabled this test because we need to implement standard testing scaffold and the
// expandContext option
//TEST(JsonLdProcessorTest, expand_0077) {
//    performExpandTest(77);
//}

TEST(JsonLdProcessorTest, expand_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performExpandTest(300);
}

TEST(JsonLdProcessorTest, expand_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performExpandTest(301);
}

TEST(JsonLdProcessorTest, expand_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performExpandTest(302);
}

