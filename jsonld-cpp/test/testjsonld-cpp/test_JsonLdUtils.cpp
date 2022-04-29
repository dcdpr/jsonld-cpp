#include <jsonld-cpp/JsonLdUtils.h>
#include <jsonld-cpp/jsoninc.h>

using nlohmann::json;

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

TEST(JsonLdUtilsTest, deepCompare_empty_object) {
    json j1 = json::parse("{}");
    json j2 = json::parse("{}");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_empty_array) {
    json j1 = json::parse("[]");
    json j2 = json::parse("[]");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_simple_true) {
    json j1 = json::parse(R"({"myproperty": { "@value" : null }})");
    json j2 = json::parse(R"({"myproperty": { "@value" : null }})");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_simple_false) {
    json j1 = json::parse(R"({"myproperty": { "@value" : null }})");
    json j2 = json::parse(R"({"myproperty": { "@value" : 1 }})");
    EXPECT_FALSE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_complex_true) {
    json j1 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    json j2 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_complex_false) {
    json j1 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    json j2 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ 1 ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    EXPECT_FALSE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_simpleTopLevelElementsOutOfOrder_true) {
    json j1 = json::parse(R"(
{
  "@id": "http://example.org/id1",
  "mylist1": []
}
)");
    json j2 = json::parse(R"(
{
  "mylist1": [],
  "@id": "http://example.org/id1"
}
)");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_complexTopLevelElementsOutOfOrder_true) {
    json j1 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    json j2 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  },
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  }
}
)");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, deepCompare_complexTopAndMidLevelElementsOutOfOrder_true) {
    json j1 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "mylist1": [],
  "mylist2": [ 2, "hi" ],
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "http://example.org/myproperty2": "ok"
  },
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ]
  }
}
)");
    json j2 = json::parse(R"(
{
  "@context": {
    "myproperty": { "@id": "http://example.com/myproperty" },
    "mylist1": {"@id": "http://example.com/mylist1", "@container": "@list"},
    "myset1": {"@id": "http://example.com/myset1", "@container": "@set" },
    "mylist2": {"@id": "http://example.com/mylist2", "@container": "@list"},
    "myset2": {"@id": "http://example.com/myset2", "@container": "@set" }
  },
  "@id": "http://example.org/id1",
  "myset1": { "@set": [] },
  "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
  "mylist2": [ 2, "hi" ],
  "mylist1": [],
  "http://example.com/emptyobj": {
    "@context": null,
    "mylist1": [],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "myset2": [ { "@set": [] }, [], [ null ], { "@set": [ null ] }]
  },
  "myproperty": {
    "@context": null,
    "@id": "http://example.org/id2",
    "mylist1": [],
    "myset2": [ { "@set": [] }, [], { "@set": [ null ] }, [ null ] ],
    "mylist2": [ 2, "hi" ],
    "myset1": { "@set": [] },
    "http://example.org/myproperty2": "ok"
  }
}
)");
    EXPECT_TRUE(JsonLdUtils::deepCompare(j1, j2));
}

TEST(JsonLdUtilsTest, test_iteration_order) {

    json j = json::object();

    j["b"] = "b"; j["key_insertion_order"].push_back("b");
    j["c"] = "c"; j["key_insertion_order"].push_back("c");
    j["a"] = "a"; j["key_insertion_order"].push_back("a");
    j["d"] = "d"; j["key_insertion_order"].push_back("d");

    // test that regular order is sorted
    json::const_iterator it = j.begin();
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

    json j = json::object();

    // test that different ways of inserting null into the json object produce the same result
    j["a"] = nullptr;
    j["b"] = json();

    EXPECT_TRUE(j["a"].is_null());
    EXPECT_TRUE(j["b"].is_null());
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_createsKeyWithSingleStringValue) {

    json j = json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_string());
    EXPECT_EQ(j["key"], "value");
}

TEST(JsonLdUtilsTest, addValue_keyDoesExistWithSingleStringValue_createsArrayAndAppendsValue) {

    json j = json::object();

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

    json j = json::object();

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

    json j = json::object();

    JsonLdUtils::addValue(j, "key", "value", true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 1);
    EXPECT_EQ(j["key"][0], "value");
}

TEST(JsonLdUtilsTest, addValue_keyDoesExist_appendsValueWithAsArrayFlag) {

    json j = json::object();

    JsonLdUtils::addValue(j, "key", "value", false);

    JsonLdUtils::addValue(j, "key", "value2", true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_appendsArrayOfValues) {

    json j = json::object();

    JsonLdUtils::addValue(j, "key", json::array({"value", "value2"}), false);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, addValue_keyDoesntExist_appendsArrayOfValuesWithAsArrayFlag) {

    json j = json::object();

    JsonLdUtils::addValue(j, "key", json::array({"value", "value2"}), true);

    EXPECT_TRUE(!j["key"].is_null());
    EXPECT_TRUE(j["key"].is_array());
    EXPECT_EQ(j["key"].size(), 2);
    EXPECT_EQ(j["key"][0], "value");
    EXPECT_EQ(j["key"][1], "value2");
}

TEST(JsonLdUtilsTest, containsOrEquals_StringVsString) {

    json j = "a";

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "a"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "b"));
}

TEST(JsonLdUtilsTest, containsOrEquals_ArrayVsString) {

    json j = json::array({"a", "b", "c"});

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "b"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "d"));
}

TEST(JsonLdUtilsTest, containsOrEquals_ObjectVsString) {

    json j = json::object({{"a", 1}, {"b", 2}, {"c", 3}});

    EXPECT_TRUE(JsonLdUtils::containsOrEquals(j, "b"));
    EXPECT_FALSE(JsonLdUtils::containsOrEquals(j, "d"));
}

TEST(JsonLdUtilsTest, isIri) {
    EXPECT_FALSE(JsonLdUtils::isIri(""));
    EXPECT_TRUE(JsonLdUtils::isIri("relative"));
    EXPECT_TRUE(JsonLdUtils::isIri("/relative"));
}

TEST(JsonLdUtilsTest, isKeywordForm) {
    EXPECT_FALSE(JsonLdUtils::isKeywordForm(""));
    EXPECT_FALSE(JsonLdUtils::isKeywordForm("@"));
    EXPECT_TRUE(JsonLdUtils::isKeywordForm("@FOO"));
    EXPECT_FALSE(JsonLdUtils::isKeywordForm("@foo.bar"));
    EXPECT_TRUE(JsonLdUtils::isKeywordForm("@ignoreMe"));
}

