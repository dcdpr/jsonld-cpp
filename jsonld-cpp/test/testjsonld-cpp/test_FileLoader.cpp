#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/FileLoader.h>
#include <jsonld-cpp/RemoteDocument.h>
#include <jsonld-cpp/RDFDataset.h>
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

TEST(DocumentLoaderTest, load_sample_json_document_from_filesystem) {

    // Loads a sample file and checks that we can read the expected content.

    FileLoader dl;

    std::string resource_dir = TEST_RESOURCE_DIR;
    std::string docPath = resource_dir + "test_data/pi-is-four.json";

    auto d = dl.loadDocument(docPath);
    nlohmann::ordered_json j = d->getJSONContent();

    EXPECT_FALSE(j == nullptr);
    EXPECT_FALSE(j.is_null());
    EXPECT_EQ(4, j["pi"]);
}

TEST(DocumentLoaderTest, load_sample_nquads_document_from_filesystem) {

    // Loads a sample file and checks that we can read the expected content.

    FileLoader dl;

    std::string resource_dir = TEST_RESOURCE_DIR;
    std::string docPath = resource_dir + "test_data/toRdf/0001-out.nq";

    auto d = dl.loadDocument(docPath);
    RDF::RDFDataset expected = d->getRDFContent();

    EXPECT_FALSE(expected.empty());
    EXPECT_EQ(expected.numTriples(), 1);
}

TEST(DocumentLoaderTest, process_duplicate_quads_when_reading) {

    // RDF datasets are not supposed to have duplicate triples/quads--if they do, we
    // can ignore the extras.

    FileLoader dl;

    std::string resource_dir = TEST_RESOURCE_DIR;
    std::string docPath = resource_dir + "test_data/duplicate_quads.nq";

    auto d = dl.loadDocument(docPath);
    RDF::RDFDataset expected = d->getRDFContent();

    EXPECT_FALSE(expected.empty());
    EXPECT_EQ(expected.numTriples(), 2);
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



