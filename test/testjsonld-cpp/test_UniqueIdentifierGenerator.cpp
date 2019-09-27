#include "UniqueIdentifierGenerator.cpp"
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

TEST(UniqueIdentifierGenTest, singleGeneration_succeeds) {
    UniqueIdentifierGenerator gen;
    std::string result = gen.generate();
    EXPECT_EQ(result, "_:b0");
}

TEST(UniqueIdentifierGenTest, twoGenerations_succeeds) {
    UniqueIdentifierGenerator gen;
    std::string result = gen.generate();
    EXPECT_EQ(result, "_:b0");
    result = gen.generate();
    EXPECT_EQ(result, "_:b1");
}

TEST(UniqueIdentifierGenTest, twoGenerations_withIdString_returnsSameId) {
    std::string name = "foo";
    UniqueIdentifierGenerator gen;
    std::string result = gen.generate(name);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name);
    EXPECT_EQ(result, "_:b0");
}

TEST(UniqueIdentifierGenTest, fourGenerations_withTwoIdStrings_returnsCorrectIds) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueIdentifierGenerator gen;

    std::string result = gen.generate(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name2);
    EXPECT_EQ(result, "_:b1");

    result = gen.generate(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.generate(name2);
    EXPECT_EQ(result, "_:b1");
}

TEST(UniqueIdentifierGenTest, identifierExists) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueIdentifierGenerator gen;

    std::string result = gen.generate(name1);
    EXPECT_TRUE(gen.exists(name1));
    EXPECT_FALSE(gen.exists(name2));
}

TEST(UniqueIdentifierGenTest, identifier_enumeration) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueIdentifierGenerator gen;

    gen.generate(name1);
    gen.generate(name2);

    auto v = gen.getIdentifiers();

    EXPECT_EQ(2, v.size());
    EXPECT_NE(v.end(), std::find(v.begin(), v.end(), name1));
    EXPECT_NE(v.end(), std::find(v.begin(), v.end(), name2));
}

TEST(UniqueIdentifierGenTest, emptyPrefix) {
    UniqueIdentifierGenerator gen("");

    std::string result = gen.generate();
    EXPECT_EQ(result, "0");
}

TEST(UniqueIdentifierGenTest, differentPrefix) {
    UniqueIdentifierGenerator gen("foo");

    std::string result = gen.generate();
    EXPECT_EQ(result, "foo0");


    std::string s = "abc";
    std::sort(s.begin(), s.end());
    do {
        std::cout << s << '\n';
    } while(std::next_permutation(s.begin(), s.end()));




}

