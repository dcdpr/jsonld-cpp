#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/detail/JsonLdUtils.h>

#include <jsonld-cpp/jsoninc.h>


#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop


TEST(JsonLdUtilsTest, test_iteration_order_basic_json) {

    nlohmann::json j = nlohmann::json::object();

    j["b"] = "b"; j["key_insertion_order"].push_back("b");
    j["c"] = "c"; j["key_insertion_order"].push_back("c");
    j["a"] = "a"; j["key_insertion_order"].push_back("a");
    j["d"] = "d"; j["key_insertion_order"].push_back("d");

    // test that regular order is sorted
    nlohmann::json::const_iterator it = j.begin();
    EXPECT_EQ("a", *it++);
    EXPECT_EQ("b", *it++);
    EXPECT_EQ("c", *it++);
    EXPECT_EQ("d", *it);

    // test manual recording of insertion order
    it = j["key_insertion_order"].begin();
    EXPECT_EQ("b", *it++);
    EXPECT_EQ("c", *it++);
    EXPECT_EQ("a", *it++);
    EXPECT_EQ("d", *it);

}

TEST(JsonLdUtilsTest, null_insertions) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    // test that different ways of inserting null into the json object produce the same result
    j["a"] = nullptr;
    j["b"] = nlohmann::ordered_json();

    EXPECT_TRUE(j["a"].is_null());
    EXPECT_TRUE(j["b"].is_null());
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_createsKeyWithSingleStringValue) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_string());
    EXPECT_EQ(j["key"], "value");
}

TEST(JsonLdUtilsTest, addValue_keyDoesExistWithSingleStringValue_createsArrayAndAppendsValue) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    EXPECT_TRUE(j["key"].is_string());
    EXPECT_EQ(j["key"], "value");

    JsonLdUtils::addValue(j, "key", "value2", false);

    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, addValue_keyDoesExistWithArrayOfValues_appendsValue) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    JsonLdUtils::addValue(j, "key", "value2", false);

    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);

    JsonLdUtils::addValue(j, "key", "value3", false);

    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 3);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
    EXPECT_EQ(j["key"][2], "value3");
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_createsArrayWithAsArrayFlag) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", "value", true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 1);
    EXPECT_EQ(j["key"][0], "value");
}

TEST(JsonLdUtilsTest, addValue_keyDoesExist_appendsValueWithAsArrayFlag) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    JsonLdUtils::addValue(j, "key", "value2", true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_appendsArrayOfValues) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", nlohmann::ordered_json::array({"value", "value2"}), false);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_appendsArrayOfValuesWithAsArrayFlag) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object();

    JsonLdUtils::addValue(j, "key", nlohmann::ordered_json::array({"value", "value2"}), true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, containsOrEquals_StringVsString) {

    nlohmann::ordered_json j = "a";

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "a"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "b"));
}

TEST(JsonLdUtilsTest, containsOrEquals_ArrayVsString) {

    nlohmann::ordered_json j = nlohmann::ordered_json::array({"a", "b", "c"});

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "b"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "d"));
}

TEST(JsonLdUtilsTest, containsOrEquals_ObjectVsString) {

    nlohmann::ordered_json j = nlohmann::ordered_json::object({{"a", 1}, {"b", 2}, {"c", 3}});

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "b"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "d"));
}

TEST(JsonLdUtilsTest, isIri) {
    EXPECT_FALSE(JsonLdUtils::isIri(""));
    EXPECT_TRUE(JsonLdUtils::isIri("relative"));
    EXPECT_TRUE(JsonLdUtils::isIri("/relative"));
    EXPECT_FALSE(JsonLdUtils::isIri("foo bar"));
    EXPECT_TRUE(JsonLdUtils::isIri("www.example.com"));
    EXPECT_TRUE(JsonLdUtils::isIri("http://www.example.com/foo"));
    EXPECT_FALSE(JsonLdUtils::isIri("http://www.example.com/foo bar"));
}

TEST(JsonLdUtilsTest, isKeywordForm) {
    EXPECT_FALSE(JsonLdUtils::isKeywordForm(""));
    EXPECT_FALSE(JsonLdUtils::isKeywordForm("@"));
    EXPECT_TRUE(JsonLdUtils::isKeywordForm("@FOO"));
    EXPECT_FALSE(JsonLdUtils::isKeywordForm("@foo.bar"));
    EXPECT_TRUE(JsonLdUtils::isKeywordForm("@ignoreMe"));
}

