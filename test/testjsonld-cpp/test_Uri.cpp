#include "Uri.h"

#include <gtest/gtest.h>

// some examples from https://tools.ietf.org/html/rfc3986#section-1.1.2

TEST(UriTest, resolveUri_rfc3986_normalExamples) {

    // from RFC 3986, Section 5.4.1

    std::string result;

    Uri::ResolveUri("http://a/b/c/d;p?q", "g", &result);
    EXPECT_EQ(result, "http://a/b/c/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "./g", &result);
    EXPECT_EQ(result, "http://a/b/c/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g/", &result);
    EXPECT_EQ(result, "http://a/b/c/g/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "/g", &result);
    EXPECT_EQ(result, "http://a/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "//g", &result);
    EXPECT_EQ(result, "http://g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "?y", &result);
    EXPECT_EQ(result, "http://a/b/c/d;p?y");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g?y", &result);
    EXPECT_EQ(result, "http://a/b/c/g?y");

    Uri::ResolveUri("http://a/b/c/d;p?q", "#s", &result);
    EXPECT_EQ(result, "http://a/b/c/d;p?q#s");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g#s", &result);
    EXPECT_EQ(result, "http://a/b/c/g#s");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g?y#s", &result);
    EXPECT_EQ(result, "http://a/b/c/g?y#s");

    Uri::ResolveUri("http://a/b/c/d;p?q", ";x", &result);
    EXPECT_EQ(result, "http://a/b/c/;x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g;x", &result);
    EXPECT_EQ(result, "http://a/b/c/g;x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g;x?y#s", &result);
    EXPECT_EQ(result, "http://a/b/c/g;x?y#s");

    Uri::ResolveUri("http://a/b/c/d;p?q", "", &result);
    EXPECT_EQ(result, "http://a/b/c/d;p?q");

    Uri::ResolveUri("http://a/b/c/d;p?q", ".", &result);
    EXPECT_EQ(result, "http://a/b/c/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "./", &result);
    EXPECT_EQ(result, "http://a/b/c/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "..", &result);
    EXPECT_EQ(result, "http://a/b/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../", &result);
    EXPECT_EQ(result, "http://a/b/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../g", &result);
    EXPECT_EQ(result, "http://a/b/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../..", &result);
    EXPECT_EQ(result, "http://a/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../../", &result);
    EXPECT_EQ(result, "http://a/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../../g", &result);
    EXPECT_EQ(result, "http://a/g");

}

TEST(UriTest, resolveUri_rfc3986_abnormalExamples) {

    // from RFC 3986, Section 5.4.2

    std::string result;

    Uri::ResolveUri("http://a/b/c/d;p?q", "../../../g", &result);
    EXPECT_EQ(result, "http://a/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "../../../../g", &result);
    EXPECT_EQ(result, "http://a/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "/./g", &result);
    EXPECT_EQ(result, "http://a/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "/../g", &result);
    EXPECT_EQ(result, "http://a/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g.", &result);
    EXPECT_EQ(result, "http://a/b/c/g.");

    Uri::ResolveUri("http://a/b/c/d;p?q", ".g", &result);
    EXPECT_EQ(result, "http://a/b/c/.g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g..", &result);
    EXPECT_EQ(result, "http://a/b/c/g..");

    Uri::ResolveUri("http://a/b/c/d;p?q", "..g", &result);
    EXPECT_EQ(result, "http://a/b/c/..g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "./../g", &result);
    EXPECT_EQ(result, "http://a/b/g");

    Uri::ResolveUri("http://a/b/c/d;p?q", "./g/.", &result);
    EXPECT_EQ(result, "http://a/b/c/g/");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g/./h", &result);
    EXPECT_EQ(result, "http://a/b/c/g/h");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g;x=1/./y", &result);
    EXPECT_EQ(result, "http://a/b/c/g;x=1/y");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g;x=1/../y", &result);
    EXPECT_EQ(result, "http://a/b/c/y");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g?y/./x", &result);
    EXPECT_EQ(result, "http://a/b/c/g?y/./x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g?y/../x", &result);
    EXPECT_EQ(result, "http://a/b/c/g?y/../x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g#s/./x", &result);
    EXPECT_EQ(result, "http://a/b/c/g#s/./x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "g#s/../x", &result);
    EXPECT_EQ(result, "http://a/b/c/g#s/../x");

    Uri::ResolveUri("http://a/b/c/d;p?q", "http:g", &result);
    EXPECT_EQ(result, "http:g");

}

TEST(UriTest, resolveUri_otherExamples) {

    std::string result;

    // discovered these as failing cases when testing expand test "#t0109", but now
    // they pass since we switched to using "external/uriparser" library

    Uri::ResolveUri("https://ex.org/", "#Test", &result);
    EXPECT_EQ(result, "https://ex.org/#Test");

    Uri::ResolveUri("https://ex.org/", "#Test:2", &result);
    EXPECT_EQ(result, "https://ex.org/#Test:2");

}


