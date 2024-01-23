#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/Uri.h>

#include <gtest/gtest.h>

TEST(UriTest, resolveUri_null_result) {

    bool resolveSucceeded;
    std::string result;

    // simple example will succeed

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g", &result);
    EXPECT_TRUE(resolveSucceeded);

    // however, if result is nullptr, then ResolveUri will fail

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g", nullptr);
    EXPECT_FALSE(resolveSucceeded);
}

TEST(UriTest, resolveUri_empty_arguments) {

    bool resolveSucceeded;
    std::string result;

    // if one or both of the uris are empty, then ResolveUri should fail

    resolveSucceeded = Uri::ResolveUri("", "", &result);
    EXPECT_FALSE(resolveSucceeded);

    resolveSucceeded = Uri::ResolveUri("a", "", &result);
    EXPECT_FALSE(resolveSucceeded);

    resolveSucceeded = Uri::ResolveUri("", "b", &result);
    EXPECT_FALSE(resolveSucceeded);

}

TEST(UriTest, resolveUri_rfc3986_normalExamples) {

    // from RFC 3986, Section 5.4.1

    bool resolveSucceeded;
    std::string result;

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "./g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g/", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "/g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "//g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "?y", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/d;p?y");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g?y", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g?y");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "#s", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/d;p?q#s");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g#s", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g#s");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g?y#s", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g?y#s");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", ";x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/;x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g;x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g;x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g;x?y#s", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g;x?y#s");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/d;p?q");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", ".", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "./", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "..", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../..", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../../", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

}

TEST(UriTest, resolveUri_rfc3986_abnormalExamples) {

    // from RFC 3986, Section 5.4.2

    bool resolveSucceeded;
    std::string result;

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../../../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "../../../../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "/./g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "/../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g.", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g.");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", ".g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/.g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g..", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g..");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "..g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/..g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "./../g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/g");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "./g/.", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g/./h", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g/h");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g;x=1/./y", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g;x=1/y");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g;x=1/../y", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/y");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g?y/./x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g?y/./x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g?y/../x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g?y/../x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g#s/./x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g#s/./x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "g#s/../x", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/b/c/g#s/../x");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/d;p?q", "http:g", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http:g");

}

TEST(UriTest, resolveUri_otherExamples) {

    bool resolveSucceeded;
    std::string result;

    // discovered these as failing cases when testing expand test "#t0109", but now
    // they pass since we switched to using "external/uriparser" library

    resolveSucceeded = Uri::ResolveUri("https://ex.org/", "#Test", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "https://ex.org/#Test");

    resolveSucceeded = Uri::ResolveUri("https://ex.org/", "#Test:2", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "https://ex.org/#Test:2");


    // discovered these as failing cases when testing toRdf test "#t0123", but now
    // they pass since we switched to using "external/uriparser" library

    resolveSucceeded = Uri::ResolveUri("http://a/bb/ccc/../d;p?q", "../..", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

    resolveSucceeded = Uri::ResolveUri("http://a/bb/ccc/../d;p?q", "../../", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/../d;p?q", "../../", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

    resolveSucceeded = Uri::ResolveUri("http://a/b/c/../d;p?q", "../..", &result);
    EXPECT_TRUE(resolveSucceeded);
    EXPECT_EQ(result, "http://a/");

}

TEST(UriTest, resolveUri_isUri) {
    EXPECT_FALSE(Uri::isUri(""));
    EXPECT_TRUE(Uri::isUri("relative"));
    EXPECT_TRUE(Uri::isUri("/relative"));
}
