#include "uri.hpp"

#include <gtest/gtest.h>

// some examples from https://tools.ietf.org/html/rfc3986#section-1.1.2

TEST(UriTest, rfc3986example1) {
    std::string a = "ftp://ftp.is.co.za/rfc/rfc1808.txt";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "ftp");
    EXPECT_EQ(parsed.get_host(), "ftp.is.co.za");
    EXPECT_EQ(parsed.get_path(), "rfc/rfc1808.txt");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example2) {
    std::string a = "http://www.ietf.org/rfc/rfc2396.txt";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "http");
    EXPECT_EQ(parsed.get_host(), "www.ietf.org");
    EXPECT_EQ(parsed.get_path(), "rfc/rfc2396.txt");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example3) {
    std::string a = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "ldap");
    EXPECT_EQ(parsed.get_host(), "[2001:db8::7]");
    EXPECT_EQ(parsed.get_path(), "c=GB");
    EXPECT_EQ(parsed.get_query(), "objectClass?one");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example4) {
    std::string a = "mailto:John.Doe@example.com";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "mailto");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "John.Doe@example.com");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example5) {
    std::string a = "news:comp.infosystems.www.servers.unix";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "news");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "comp.infosystems.www.servers.unix");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example6) {
    std::string a = "tel:+1-816-555-1212";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "tel");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "+1-816-555-1212");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example7) {
    std::string a = "telnet://192.0.2.16:80/";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "telnet");
    EXPECT_EQ(parsed.get_host(), "192.0.2.16");
    EXPECT_EQ(parsed.get_port(), 80);
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, rfc3986example8) {
    std::string a = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
    uri parsed(a);

    EXPECT_EQ(parsed.get_scheme(), "urn");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "");
}

// other tests

TEST(UriTest, missing_scheme) {
    std::string a = "www.example.com/one?two#three";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "www.example.com/one");
    EXPECT_EQ(parsed.get_query(), "two");
    EXPECT_EQ(parsed.get_fragment(), "three");
}

TEST(UriTest, path_and_query) {
    std::string a = "one?two";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "one");
    EXPECT_EQ(parsed.get_query(), "two");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, query_and_fragment) {
    std::string a = "?two#three";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "two");
    EXPECT_EQ(parsed.get_fragment(), "three");
}

TEST(UriTest, just_query) {
    std::string a = "?two";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "two");
    EXPECT_EQ(parsed.get_fragment(), "");
}

TEST(UriTest, just_fragment) {
    std::string a = "#three";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "three");
}

TEST(UriTest, fragment_with_colon) {
    std::string a = "http://www.example.com/one?two#three:a";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "http");
    EXPECT_EQ(parsed.get_host(), "www.example.com");
    EXPECT_EQ(parsed.get_path(), "one");
    EXPECT_EQ(parsed.get_query(), "two");
    EXPECT_EQ(parsed.get_fragment(), "three:a");
}

TEST(UriTest, just_fragment_with_colon) {
    std::string a = "#three:a";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "three:a");
}

TEST(UriTest, just_fragment_with_escaped_colon) {
    std::string a = "#three%3aa";
    uri parsed(a);

    // not sure if this is what should happen, but I'm just exploring how the library works
    EXPECT_EQ(parsed.get_scheme(), "");
    EXPECT_EQ(parsed.get_host(), "");
    EXPECT_EQ(parsed.get_path(), "");
    EXPECT_EQ(parsed.get_query(), "");
    EXPECT_EQ(parsed.get_fragment(), "three%3aa");
}



