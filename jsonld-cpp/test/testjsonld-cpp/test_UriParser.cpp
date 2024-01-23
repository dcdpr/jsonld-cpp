#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/UriParser.h>

#include <gtest/gtest.h>


// some basic tests

TEST(UriParserTest, empty_uri) {
    std::string a = "";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, simple_string) {
    std::string a = "example";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "example");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}



// some examples from https://tools.ietf.org/html/rfc3986#section-1.1.2

TEST(UriParserTest, rfc3986example1) {
    std::string a = "ftp://ftp.is.co.za/rfc/rfc1808.txt";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "ftp");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "ftp.is.co.za");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "rfc/rfc1808.txt");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example2) {
    std::string a = "http://www.ietf.org/rfc/rfc2396.txt";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "http");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "www.ietf.org");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "rfc/rfc2396.txt");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example3) {
    std::string a = "ldap://[2001:db8::7]/c=GB?objectClass?one";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "ldap");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "2001:db8::7");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "c=GB");
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "objectClass?one");
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example4) {
    std::string a = "mailto:John.Doe@example.com";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "mailto");
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "John.Doe@example.com");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example5) {
    std::string a = "news:comp.infosystems.www.servers.unix";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "news");
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "comp.infosystems.www.servers.unix");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example6) {
    std::string a = "tel:+1-816-555-1212";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "tel");
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "+1-816-555-1212");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example7) {
    std::string a = "telnet://192.0.2.16:80/";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: path exists, even though it is empty string

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "telnet");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "192.0.2.16");
    EXPECT_TRUE(base_uri->GetPort(&t));
    EXPECT_EQ(t, "80");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example7_no_path) {
    std::string a = "telnet://192.0.2.16:80";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: due to there not being a final "/", the path does
    // not exist, not even as an empty string

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "telnet");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "192.0.2.16");
    EXPECT_TRUE(base_uri->GetPort(&t));
    EXPECT_EQ(t, "80");
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986example8) {
    std::string a = "urn:oasis:names:specification:docbook:dtd:xml:4.1.2";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "urn");
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "oasis:names:specification:docbook:dtd:xml:4.1.2");
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, rfc3986_reference_example) {
    std::string a = "http://a/b/c/d;p?q";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "http");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "a");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "b/c/d;p");
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "q");
    EXPECT_FALSE(base_uri->GetFragment(&t));
}


// other tests

TEST(UriParserTest, missing_scheme) {
    std::string a = "www.example.com/one?two#three";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: what could be the host becomes part of the path

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "www.example.com/one");
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "two");
    EXPECT_TRUE(base_uri->GetFragment(&t));
    EXPECT_EQ(t, "three");
}

TEST(UriParserTest, path_and_query) {
    std::string a = "one?two";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: only path and query is detected

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "one");
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "two");
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, query_and_fragment) {
    std::string a = "?two#three";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: only query and fragment is detected

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "two");
    EXPECT_TRUE(base_uri->GetFragment(&t));
    EXPECT_EQ(t, "three");
}

TEST(UriParserTest, just_query) {
    std::string a = "?two";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: only query is detected

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "two");
    EXPECT_FALSE(base_uri->GetFragment(&t));
}

TEST(UriParserTest, just_fragment) {
    std::string a = "#three";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    // note: only fragment is detected

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_TRUE(base_uri->GetFragment(&t));
    EXPECT_EQ(t, "three");
}

TEST(UriParserTest, fragment_with_colon) {
    std::string a = "http://www.example.com/one?two#three:a";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_TRUE(base_uri->GetScheme(&t));
    EXPECT_EQ(t, "http");
    EXPECT_TRUE(base_uri->GetHost(&t));
    EXPECT_EQ(t, "www.example.com");
    EXPECT_TRUE(base_uri->GetPath(&t));
    EXPECT_EQ(t, "one");
    EXPECT_TRUE(base_uri->GetQuery(&t));
    EXPECT_EQ(t, "two");
    EXPECT_TRUE(base_uri->GetFragment(&t));
    EXPECT_EQ(t, "three:a");
}

TEST(UriParserTest, just_fragment_with_colon) {
    std::string a = "#three:a";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    std::string t;
    EXPECT_FALSE(base_uri->GetScheme(&t));
    EXPECT_FALSE(base_uri->GetHost(&t));
    EXPECT_FALSE(base_uri->GetPath(&t));
    EXPECT_FALSE(base_uri->GetQuery(&t));
    EXPECT_TRUE(base_uri->GetFragment(&t));
    EXPECT_EQ(t, "three:a");
}

TEST(UriParserNormalizeTest, normalize_empty_returns_empty) {
    std::string a = "";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("", t);
}

TEST(UriParserNormalizeTest, normalize_simple_returns_same) {
    std::string a = "example.com";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example.com", t);
}

TEST(UriParserNormalizeTest, normalize_withLeadingingSlash_returnSame) {
    std::string a = "/example";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("/example", t);
}

TEST(UriParserNormalizeTest, normalize_withSingleDotAtStart_returnsRemoved) {
    std::string a = "./example";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example", t);
}

TEST(UriParserNormalizeTest, normalize_withMultipleSingleDotsAtStart_returnsRemoved) {
    std::string a = "./././example";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example", t);
}

TEST(UriParserNormalizeTest, normalize_withSingleDotInMiddle_returnsRemoved) {
    std::string a = "example/./foo";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example/foo", t);
}

TEST(UriParserNormalizeTest, normalize_withDoubleDotsInMiddle_returnsRemoved) {
    std::string a = "example/../foo";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("foo", t);
}

TEST(UriParserNormalizeTest, normalize_withManySingleDotsInMiddle_returnsRemoved) {
    std::string a = "example/./foo/./bar";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example/foo/bar", t);
}

TEST(UriParserNormalizeTest, normalize_withManyDoubleDotsInMiddle_returnsRemoved) {
    std::string a = "example/../foo/../bar";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("bar", t);
}

TEST(UriParserNormalizeTest, normalize_withSingleDotAtEnd_returnsRemoved) {
    std::string a = "example/.";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("example/", t);
}

TEST(UriParserNormalizeTest, normalize_rfc3986_example1) {
    std::string a = "/a/b/c/./../../g";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("/a/g", t);
}

TEST(UriParserNormalizeTest, normalize_rfc3986_example2) {
    std::string a = "mid/content=5/../6";
    std::unique_ptr<UriParser> base_uri(UriParser::create(a.c_str()));

    EXPECT_TRUE(base_uri->Normalize());

    std::string t;
    EXPECT_TRUE(base_uri->ToString(&t));

    EXPECT_EQ("mid/6", t);
}

TEST(UriParserResolveTest, resolve_empty_args) {
    std::unique_ptr<UriParser> resolved_uri(
            UriParser::createResolved("", ""));

    EXPECT_FALSE(resolved_uri);
}

TEST(UriParserResolveTest, resolve_empty_baseUri) {
    std::unique_ptr<UriParser> resolved_uri(
            UriParser::createResolved("", "example"));

    EXPECT_FALSE(resolved_uri);
}

TEST(UriParserResolveTest, resolve_empty_relativeUri) {
    std::unique_ptr<UriParser> resolved_uri(
            UriParser::createResolved("http://www.example.com", ""));

    EXPECT_TRUE(resolved_uri);
    std::string t;
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ("http://www.example.com", t);
}

TEST(UriParserResolveTest, resolve_rfc3986_normalExamples) {
    std::unique_ptr<UriParser> resolved_uri;
    std::string t;

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q", "g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ("http://a/b/c/g", t);


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q", "./g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ("http://a/b/c/g", t);


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g/"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "/g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "//g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://g");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "?y"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/d;p?y");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g?y"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g?y");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "#s"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/d;p?q#s");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g#s"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g#s");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g?y#s"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g?y#s");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  ";x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/;x");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g;x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g;x");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g;x?y#s"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g;x?y#s");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  ""));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/d;p?q");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "./"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  ".."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/g");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../.."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../../"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");


    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");

}

TEST(UriParserResolveTest, resolve_rfc3986_abnormalExamples) {
    std::unique_ptr<UriParser> resolved_uri;
    std::string t;

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../../../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "../../../../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "/./g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "/../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g.");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  ".g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/.g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g.."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g..");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "..g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/..g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "./../g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/g");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "./g/."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g/");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g/./h"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g/h");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g;x=1/./y"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g;x=1/y");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g;x=1/../y"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/y");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g?y/./x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g?y/./x");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g?y/../x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g?y/../x");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g#s/./x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g#s/./x");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "g#s/../x"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/b/c/g#s/../x");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/d;p?q",  "http:g"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http:g");
}

TEST(UriParserResolveTest, resolve_otherExamples) {
    std::unique_ptr<UriParser> resolved_uri;
    std::string t;

    // discovered these as failing cases when testing expand test "#t0109", but now
    // they pass since we switched to using "external/uriparser" library

    resolved_uri.reset(UriParser::createResolved("https://ex.org/",  "#Test"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "https://ex.org/#Test");

    resolved_uri.reset(UriParser::createResolved("https://ex.org/",  "#Test:2"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "https://ex.org/#Test:2");

    // discovered these as failing cases when testing toRdf test "#t0123", but now
    // they pass since we switched to using "external/uriparser" library

    resolved_uri.reset(UriParser::createResolved("http://a/bb/ccc/../d;p?q", "../.."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");

    resolved_uri.reset(UriParser::createResolved("http://a/bb/ccc/../d;p?q", "../../"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/../d;p?q", "../../"));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");

    resolved_uri.reset(UriParser::createResolved("http://a/b/c/../d;p?q", "../.."));
    EXPECT_TRUE(resolved_uri);
    EXPECT_TRUE(resolved_uri->ToString(&t));
    EXPECT_EQ(t, "http://a/");

}
