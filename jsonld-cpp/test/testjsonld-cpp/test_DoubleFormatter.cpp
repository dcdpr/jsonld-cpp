#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/DoubleFormatter.h>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

TEST(DoubleFormatterTest, simple) {
    double d = 5.3;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "5.3E0");
}

TEST(DoubleFormatterTest, simple_positive4) {
    double d = 53000;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "5.3E4");
}

TEST(DoubleFormatterTest, simple_negative) {
    double d = 0.53;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "5.3E-1");
}

TEST(DoubleFormatterTest, simple_negative2) {
    double d = 0.053;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "5.3E-2");
}

TEST(DoubleFormatterTest, simple_negative13) {
    double d = 0.00000000000053;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "5.3E-13");
}

TEST(DoubleFormatterTest, simple_one) {
    double d = 1;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "1.0E0");
}

TEST(DoubleFormatterTest, simple_ten) {
    double d = 10;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "1.0E1");
}

TEST(DoubleFormatterTest, simple_neg_ten) {
    double d = -10;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "-1.0E1");
}

TEST(DoubleFormatterTest, simple_messy) {
    double d = 98340011.00004567;
    std::string s = DoubleFormatter::format(d);
    EXPECT_EQ(s, "9.834001100004567E7");
}



