#pragma ide diagnostic ignored "cert-err58-cpp"
#include "jsonld-cpp/NQuadsSerialization.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <fstream>
#include <regex>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

using namespace RDF;

TEST(RegexTest, regexp_replace) {

    std::string str = "abcdef";
    std::string m = "abc";
    std::regex pat(m);
    str = std::regex_replace(str, pat, "X");

    EXPECT_EQ("Xdef", str);

}

TEST(RegexTest, regexp_replace_uchars) {

    std::string str = "\\u0022\\u005c";
    std::string m = "\\u0022";
    std::regex pat(NQuadsSerialization::escape(m));
    str = std::regex_replace(str, pat, "X");

    EXPECT_EQ("X\\u005c", str);

}

TEST(RegexTest, unescape_uchars) {
    std::string str = "\\u0022\\u005c";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"\\", result);
}

TEST(RegexTest, unescape_slashes1) {
    // in this case, unescape does nothing because a single backslash is not 'escaping' anything
    std::string str = "\\";
    std::string result = NQuadsSerialization::unescape(str);
    // in fact, unescape simply returns the input string without any processing
    EXPECT_EQ("\\", result);
}

TEST(RegexTest, unescape_slashes2) {
    // in this case, the input string represents an escaped backslash (one
    // backslash which is escaped here to be represented in a string, and
    // a preceding backslash, also escaped here to be represented in a string)
    std::string str = "\\\\";
    std::string result = NQuadsSerialization::unescape(str);
    // returns a single backslash (escaped since represented in a string)
    EXPECT_EQ("\\", result);
}

TEST(RegexTest, unescape_slashes2a) {
    // in this case, the input string represents an escaped backslash, the
    // letter a, and another escaped backslash
    std::string str = "\\\\a\\\\";
    std::string result = NQuadsSerialization::unescape(str);
    // returns a backslash, the letter a, and another escaped backslash
    EXPECT_EQ("\\a\\", result);
}

TEST(RegexTest, unescape_slashes3) {
    // in this case, the input string represents three escaped backslashes
    std::string str = "\\\\\\"; // not really valid as three backslashes in a row isn't a valid c++ string
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\\\\", result);
}

TEST(RegexTest, unescape_slashes4) {
    std::string str = "\\\\\\\\";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\\\\", result);
}

TEST(RegexTest, unescape_slashes5) {
    std::string str = "\\\\\\\\\\"; // not really valid as five backslashes in a row isn't a valid c++ string
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\\\\\\", result);
}

TEST(RegexTest, unescape_slashes6) {
    std::string str = "\\\\\\\\\\\\";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\\\\\\", result);
}

TEST(RegexTest, unescape_dquote) {
    std::string str = "\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"", result);
}

TEST(RegexTest, unescape_dquote2) {
    std::string str = "\"\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"\"", result);
}

TEST(RegexTest, unescape_dquote3) {
    std::string str = "\"\"\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"\"\"", result);
}

TEST(RegexTest, unescape_slashes_dquote1) {
    std::string str = "\\\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"", result);
}

TEST(RegexTest, unescape_slashes) {
    std::string str = "\\\"literals\\\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("\"literals\"", result);
}

TEST(RegexTest, unescape_slashes_lots) {
    std::string str = "B\\\"\\\\\\\\\\\"/\"";
    std::string result = NQuadsSerialization::unescape(str);
    EXPECT_EQ("B\"\\\\\"/\"", result);
}
