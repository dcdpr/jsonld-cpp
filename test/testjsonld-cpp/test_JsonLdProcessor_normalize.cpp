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

void performNormalizeTest(int testNumber) {

    std::string testName = "normalize";
    std::string testNumberStr = getTestNumberStr(testNumber);

    std::string baseUri = getBaseUri(testName, testNumberStr);
    std::string inputStr = getInputStr(testName, testNumberStr);
    std::string expected = getExpectedRDF(testName, testNumberStr);

    DocumentLoader dl;
    dl.addDocumentToCache(baseUri, inputStr);
    JsonLdOptions opts(baseUri);
    opts.setDocumentLoader(dl);

    std::string str = JsonLdProcessor::normalize(baseUri, opts);

    EXPECT_EQ(expected, str);
}

TEST(JsonLdProcessorTest, normalize_0001) {
    performNormalizeTest(1);
}

TEST(JsonLdProcessorTest, normalize_0002) {
    performNormalizeTest(2);
}

TEST(JsonLdProcessorTest, normalize_0003) {
    performNormalizeTest(3);
}

TEST(JsonLdProcessorTest, normalize_0004) {
    performNormalizeTest(4);
}

TEST(JsonLdProcessorTest, normalize_0005) {
    performNormalizeTest(5);
}

TEST(JsonLdProcessorTest, normalize_0006) {
    performNormalizeTest(6);
}

TEST(JsonLdProcessorTest, normalize_0007) {
    performNormalizeTest(7);
}

TEST(JsonLdProcessorTest, normalize_0008) {
    performNormalizeTest(8);
}

TEST(JsonLdProcessorTest, normalize_0009) {
    performNormalizeTest(9);
}

TEST(JsonLdProcessorTest, normalize_0010) {
    performNormalizeTest(10);
}

TEST(JsonLdProcessorTest, normalize_0011) {
    performNormalizeTest(11);
}

TEST(JsonLdProcessorTest, normalize_0012) {
    performNormalizeTest(12);
}

TEST(JsonLdProcessorTest, normalize_0013) {
    performNormalizeTest(13);
}

TEST(JsonLdProcessorTest, normalize_0014) {
    performNormalizeTest(14);
}

TEST(JsonLdProcessorTest, normalize_0015) {
    performNormalizeTest(15);
}

TEST(JsonLdProcessorTest, normalize_0016) {
    performNormalizeTest(16);
}

TEST(JsonLdProcessorTest, normalize_0017) {
    performNormalizeTest(17);
}

TEST(JsonLdProcessorTest, normalize_0018) {
    performNormalizeTest(18);
}

TEST(JsonLdProcessorTest, normalize_0019) {
    performNormalizeTest(19);
}

TEST(JsonLdProcessorTest, normalize_0020) {
    performNormalizeTest(20);
}

TEST(JsonLdProcessorTest, normalize_0021) {
    performNormalizeTest(21);
}

TEST(JsonLdProcessorTest, normalize_0022) {
    performNormalizeTest(22);
}

TEST(JsonLdProcessorTest, normalize_0023) {
    performNormalizeTest(23);
}

TEST(JsonLdProcessorTest, normalize_0024) {
    performNormalizeTest(24);
}

TEST(JsonLdProcessorTest, normalize_0025) {
    performNormalizeTest(25);
}

TEST(JsonLdProcessorTest, normalize_0026) {
    performNormalizeTest(26);
}

TEST(JsonLdProcessorTest, normalize_0027) {
    performNormalizeTest(27);
}

TEST(JsonLdProcessorTest, normalize_0028) {
    performNormalizeTest(28);
}

TEST(JsonLdProcessorTest, normalize_0029) {
    performNormalizeTest(29);
}

TEST(JsonLdProcessorTest, normalize_0030) {
    performNormalizeTest(30);
}

TEST(JsonLdProcessorTest, normalize_0031) {
    performNormalizeTest(31);
}

TEST(JsonLdProcessorTest, normalize_0032) {
    performNormalizeTest(32);
}

TEST(JsonLdProcessorTest, normalize_0033) {
    performNormalizeTest(33);
}

TEST(JsonLdProcessorTest, normalize_0034) {
    performNormalizeTest(34);
}

TEST(JsonLdProcessorTest, normalize_0035) {
    performNormalizeTest(35);
}

TEST(JsonLdProcessorTest, normalize_0036) {
    performNormalizeTest(36);
}

TEST(JsonLdProcessorTest, normalize_0037) {
    performNormalizeTest(37);
}

TEST(JsonLdProcessorTest, normalize_0038) {
    performNormalizeTest(38);
}

TEST(JsonLdProcessorTest, normalize_0039) {
    performNormalizeTest(39);
}

TEST(JsonLdProcessorTest, normalize_0040) {
    performNormalizeTest(40);
}

TEST(JsonLdProcessorTest, normalize_0041) {
    performNormalizeTest(41);
}

TEST(JsonLdProcessorTest, normalize_0042) {
    performNormalizeTest(42);
}

TEST(JsonLdProcessorTest, normalize_0043) {
    performNormalizeTest(43);
}

TEST(JsonLdProcessorTest, normalize_0044) {
    performNormalizeTest(44);
}

TEST(JsonLdProcessorTest, normalize_0045) {
    performNormalizeTest(45);
}

TEST(JsonLdProcessorTest, normalize_0046) {
    performNormalizeTest(46);
}

TEST(JsonLdProcessorTest, normalize_0047) {
    performNormalizeTest(47);
}

TEST(JsonLdProcessorTest, normalize_0048) {
    performNormalizeTest(48);
}

TEST(JsonLdProcessorTest, normalize_0049) {
    performNormalizeTest(49);
}

TEST(JsonLdProcessorTest, normalize_0050) {
    performNormalizeTest(50);
}

TEST(JsonLdProcessorTest, normalize_0051) {
    performNormalizeTest(51);
}

TEST(JsonLdProcessorTest, normalize_0052) {
    performNormalizeTest(52);
}

TEST(JsonLdProcessorTest, normalize_0053) {
    performNormalizeTest(53);
}

TEST(JsonLdProcessorTest, normalize_0054) {
    performNormalizeTest(54);
}

TEST(JsonLdProcessorTest, normalize_0055) {
    performNormalizeTest(55);
}

TEST(JsonLdProcessorTest, normalize_0056) {
    performNormalizeTest(56);
}

TEST(JsonLdProcessorTest, normalize_0057) {
    performNormalizeTest(57);
}

