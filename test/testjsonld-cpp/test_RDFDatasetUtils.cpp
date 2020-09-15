#include "RDFDatasetUtils.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <fstream>
#include <ObjUtils.h>
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
    std::regex pat(RDFDatasetUtils::escape(m));
    str = std::regex_replace(str, pat, "X");

    EXPECT_EQ("X\\u005c", str);

}

TEST(RegexTest, unescape) {

    std::string str = "\\u0022\\u005c";

    std::string result = RDFDatasetUtils::unescape(str);

    EXPECT_EQ("\"\\", result);

}

