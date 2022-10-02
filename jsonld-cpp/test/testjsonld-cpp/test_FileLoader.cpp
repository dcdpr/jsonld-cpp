#include <jsonld-cpp/FileLoader.h>
#include <jsonld-cpp/RemoteDocument.h>
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

TEST(DocumentLoaderTest, load_sample_document_from_filesystem) {
    FileLoader dl;

    std::string resource_dir = TEST_RESOURCE_DIR;
    std::string docPath = resource_dir + "test_data/pi-is-four.json";

    auto d = dl.loadDocument(docPath);
    json j = d->getJSONContent();
    EXPECT_FALSE(j == nullptr);
    EXPECT_FALSE(j.is_null());
    EXPECT_EQ(4, j["pi"]);
}

// todo: need to re-implement cache for these tests to be meaningful
//TEST(DocumentLoaderTest, load_sample_document_from_cache) {
//    FileLoader dl;
//    dl.addDocumentToCache("foo.json", R"({ "pi": 3 })");
//
//    RemoteDocument d = dl.loadDocument("foo.json");
//    EXPECT_EQ(3, d.getJSONContent()["pi"]);
//}
//
//TEST(DocumentLoaderTest, load_document_from_cache_miss) {
//    FileLoader dl;
//    dl.addDocumentToCache("foo.json", R"({ "pi": 3 })");
//
//    EXPECT_THROW(dl.loadDocument("bar.json"), std::runtime_error);
//}



