#include <jsonld-cpp/UniqueNamer.h>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

TEST(UniqueNamerGenTest, singleGeneration_succeeds) {
    UniqueNamer gen;
    std::string result = gen.get();
    EXPECT_EQ(result, "_:b0");
}

TEST(UniqueNamerGenTest, twoGenerations_succeeds) {
    UniqueNamer gen;
    std::string result = gen.get();
    EXPECT_EQ(result, "_:b0");
    result = gen.get();
    EXPECT_EQ(result, "_:b1");
}

TEST(UniqueNamerGenTest, twoGenerations_withIdString_returnsSameId) {
    std::string name = "foo";
    UniqueNamer gen;
    std::string result = gen.get(name);
    EXPECT_EQ(result, "_:b0");
    result = gen.get(name);
    EXPECT_EQ(result, "_:b0");
}

TEST(UniqueNamerGenTest, fourGenerations_withTwoIdStrings_returnsCorrectIds) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueNamer gen;

    std::string result = gen.get(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.get(name2);
    EXPECT_EQ(result, "_:b1");

    result = gen.get(name1);
    EXPECT_EQ(result, "_:b0");
    result = gen.get(name2);
    EXPECT_EQ(result, "_:b1");
}

TEST(UniqueNamerGenTest, nameExists) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueNamer gen;

    std::string result = gen.get(name1);
    EXPECT_TRUE(gen.exists(name1));
    EXPECT_FALSE(gen.exists(name2));
}

TEST(UniqueNamerGenTest, name_enumeration) {
    std::string name1 = "foo";
    std::string name2 = "bar";
    UniqueNamer gen;

    gen.get(name1);
    gen.get(name2);

    auto v = gen.getKeys();

    EXPECT_EQ(2, v.size());
    EXPECT_NE(v.end(), std::find(v.begin(), v.end(), name1));
    EXPECT_NE(v.end(), std::find(v.begin(), v.end(), name2));
}

TEST(UniqueNamerGenTest, emptyPrefix) {
    UniqueNamer gen("");

    std::string result = gen.get();
    EXPECT_EQ(result, "0");
}

TEST(UniqueNamerGenTest, differentPrefix) {
    UniqueNamer gen("foo");

    std::string result = gen.get();
    EXPECT_EQ(result, "foo0");
}

