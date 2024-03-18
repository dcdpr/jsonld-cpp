#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/JsonLdUrl.h>

#include <gtest/gtest.h>

TEST(JsonLdUrlTest, resolve_example) {

    std::string base = "http://a/b/c/d;p?q";
    std::string path = "../g";
    std::string result = JsonLdUrl::resolve(&base, &path);

    EXPECT_EQ(result, "http://a/b/g");
}

TEST(JsonLdUrlTest, resolve_null_base) {

    std::string path = "../g";
    std::string result = JsonLdUrl::resolve(nullptr, &path);

    EXPECT_EQ(result, path);
}

TEST(JsonLdUrlTest, resolve_null_path) {

    std::string base = "http://a/b/c/d;p?q";
    std::string result = JsonLdUrl::resolve(&base, nullptr);

    EXPECT_EQ(result, base);
}

TEST(JsonLdUrlTest, resolve_null_args) {

    std::string result = JsonLdUrl::resolve(nullptr, nullptr);

    EXPECT_TRUE(result.empty());
}
