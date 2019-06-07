#include "JsonLdApi.cpp"
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

TEST(JsonLdApiTest, construct_default) {
    JsonLdApi api;
    EXPECT_EQ(api.getOptions().getBase(), "");
}

TEST(JsonLdApiTest, construct_with_options) {
//    auto options = std::shared_ptr<JsonLdOptions>(new JsonLdOptions("foobar"));
    JsonLdOptions options("foobar");
    JsonLdApi api(options);
    EXPECT_EQ(api.getOptions().getBase(), "foobar");
}



TEST(JsonLdApiTest, json_string_test) {
    json jstring = "http://www.example.com/somedata.json";
    std::cout << jstring << std::endl;
    json jobject = json::object();
    std::cout << jobject << std::endl;
    json jarray = json::array();
    std::cout << jarray << std::endl;

}



