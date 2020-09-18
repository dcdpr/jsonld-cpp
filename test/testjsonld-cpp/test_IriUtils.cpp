#include "IriUtils.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

TEST(IriUtilsTest, removeDotSegments_fromEmpty_returnEmpty) {
    std::string result = IriUtils::removeDotSegments("", false);
    EXPECT_EQ(result, "");
}

TEST(IriUtilsTest, removeDotSegments_withNoDots_returnSame) {
    std::string result = IriUtils::removeDotSegments("example", false);
    EXPECT_EQ(result, "example");
}

TEST(IriUtilsTest, removeDotSegments_withLeadingingSlash_returnSame) {
    std::string result = IriUtils::removeDotSegments("/example", false);
    EXPECT_EQ(result, "/example");
}

TEST(IriUtilsTest, removeDotSegments_withSingleDotAtStart_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("./example", false);
    EXPECT_EQ(result, "example");
}

TEST(IriUtilsTest, removeDotSegments_withMultipleSingleDotsAtStart_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("./././example", false);
    EXPECT_EQ(result, "example");
}

TEST(IriUtilsTest, removeDotSegments_withDoubleDotsAtStart_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("../example", false);
    EXPECT_EQ(result, "example");
}

TEST(IriUtilsTest, removeDotSegments_withMultipleDoubleDotsAtStart_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("../../../example", false);
    EXPECT_EQ(result, "example");
}

TEST(IriUtilsTest, removeDotSegments_withSingleDotInMiddle_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("example/./foo", false);
    EXPECT_EQ(result, "example/foo");
}

TEST(IriUtilsTest, removeDotSegments_withDoubleDotsInMiddle_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("example/../foo", false);
    EXPECT_EQ(result, "foo");
}

TEST(IriUtilsTest, removeDotSegments_withManySingleDotsInMiddle_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("example/./foo/./bar", false);
    EXPECT_EQ(result, "example/foo/bar");
}

TEST(IriUtilsTest, removeDotSegments_withManyDoubleDotsInMiddle_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("example/../foo/../bar", false);
    EXPECT_EQ(result, "bar");
}

TEST(IriUtilsTest, removeDotSegments_withSingleDotAtEnd_returnsRemoved) {
    std::string result = IriUtils::removeDotSegments("example/.", false);
    EXPECT_EQ(result, "example/");
}

TEST(IriUtilsTest, removeDotSegments_rfc3986_example1) {
    std::string result = IriUtils::removeDotSegments("/a/b/c/./../../g", false);
    EXPECT_EQ(result, "/a/g");
}

TEST(IriUtilsTest, removeDotSegments_rfc3986_example2) {
    std::string result = IriUtils::removeDotSegments("mid/content=5/../6", false);
    EXPECT_EQ(result, "mid/6");
}

TEST(IriUtilsTest, prependBase_emptyArguments) {
    std::string result;

    result = IriUtils::prependBase("", "");
    EXPECT_EQ(result, "");

    result = IriUtils::prependBase("", "b");
    EXPECT_EQ(result, "b");
}

TEST(IriUtilsTest, prependBase_rfc3986_normalExamples) {

    // from RFC 3986, Section 5.4.1

    std::string result;

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g");
    EXPECT_EQ(result, "http://a/b/c/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "./g");
    EXPECT_EQ(result, "http://a/b/c/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g/");
    EXPECT_EQ(result, "http://a/b/c/g/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "/g");
    EXPECT_EQ(result, "http://a/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "//g");
    EXPECT_EQ(result, "http://g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "?y");
    EXPECT_EQ(result, "http://a/b/c/d;p?y");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g?y");
    EXPECT_EQ(result, "http://a/b/c/g?y");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "#s");
    EXPECT_EQ(result, "http://a/b/c/d;p?q#s");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g#s");
    EXPECT_EQ(result, "http://a/b/c/g#s");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g?y#s");
    EXPECT_EQ(result, "http://a/b/c/g?y#s");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", ";x");
    EXPECT_EQ(result, "http://a/b/c/;x");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g;x");
    EXPECT_EQ(result, "http://a/b/c/g;x");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g;x?y#s");
    EXPECT_EQ(result, "http://a/b/c/g;x?y#s");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "");
    EXPECT_EQ(result, "http://a/b/c/d;p?q");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", ".");
    EXPECT_EQ(result, "http://a/b/c/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "./");
    EXPECT_EQ(result, "http://a/b/c/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "..");
    EXPECT_EQ(result, "http://a/b/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../");
    EXPECT_EQ(result, "http://a/b/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../g");
    EXPECT_EQ(result, "http://a/b/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../..");
    EXPECT_EQ(result, "http://a/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../../");
    EXPECT_EQ(result, "http://a/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../../g");
    EXPECT_EQ(result, "http://a/g");
}

TEST(IriUtilsTest, prependBase_rfc3986_abnormalExamples) {

    // from RFC 3986, Section 5.4.2

    std::string result;

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../../../g");
    EXPECT_EQ(result, "http://a/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "../../../../g");
    EXPECT_EQ(result, "http://a/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "/./g");
    EXPECT_EQ(result, "http://a/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "/../g");
    EXPECT_EQ(result, "http://a/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g.");
    EXPECT_EQ(result, "http://a/b/c/g.");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", ".g");
    EXPECT_EQ(result, "http://a/b/c/.g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g..");
    EXPECT_EQ(result, "http://a/b/c/g..");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "..g");
    EXPECT_EQ(result, "http://a/b/c/..g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "./../g");
    EXPECT_EQ(result, "http://a/b/g");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "./g/.");
    EXPECT_EQ(result, "http://a/b/c/g/");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g/./h");
    EXPECT_EQ(result, "http://a/b/c/g/h");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g;x=1/./y");
    EXPECT_EQ(result, "http://a/b/c/g;x=1/y");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g;x=1/../y");
    EXPECT_EQ(result, "http://a/b/c/y");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g?y/./x");
    EXPECT_EQ(result, "http://a/b/c/g?y/./x");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g?y/../x");
    EXPECT_EQ(result, "http://a/b/c/g?y/../x");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g#s/./x");
    EXPECT_EQ(result, "http://a/b/c/g#s/./x");

    result = IriUtils::prependBase("http://a/b/c/d;p?q", "g#s/../x");
    EXPECT_EQ(result, "http://a/b/c/g#s/../x");

//    result = IriUtils::prependBase("http://a/b/c/d;p?q", "http:g");
//    EXPECT_EQ(result, "http:g");

}

TEST(IriUtilsTest, prependBase_otherExamples) {

    std::string result;

    // discovered this as a failing case when testing expand test "#t0109"
    result = IriUtils::prependBase("https://ex.org/", "#Test");
    EXPECT_EQ(result, "https://ex.org/#Test");

}
