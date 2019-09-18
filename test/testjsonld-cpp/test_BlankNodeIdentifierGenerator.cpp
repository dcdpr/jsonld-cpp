#include "BlankNodeIdentifierGenerator.cpp"
#include "testHelpers.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

TEST(BNIDGenTest, singleGeneration_succeeds) {
    BlankNodeIdentifierGenerator gen;
    std::string result = gen.generate();
    EXPECT_EQ(result, "_:b0");
}

TEST(BNIDGenTest, twoGenerations_succeeds) {
    BlankNodeIdentifierGenerator gen;
    std::string result = gen.generate();
    EXPECT_EQ(result, "_:b0");
    result = gen.generate();
    EXPECT_EQ(result, "_:b1");
}

TEST(BNIDGenTest, twoGenerations_withIdString_returnsSameId) {
    std::string name = "foo";
    BlankNodeIdentifierGenerator gen;
    std::string result = gen.generate(name);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name);
    EXPECT_EQ(result, "_:b0");
}

TEST(BNIDGenTest, fourGenerations_withTwoIdStrings_returnsCorrectIds) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    BlankNodeIdentifierGenerator gen;

    std::string result = gen.generate(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name2);
    EXPECT_EQ(result, "_:b1");

    result = gen.generate(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name2);
    EXPECT_EQ(result, "_:b1");
}

