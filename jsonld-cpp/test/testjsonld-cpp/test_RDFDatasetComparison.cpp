#include <jsonld-cpp/RDFDataset.h>
#include <jsonld-cpp/RDFDatasetComparison.h>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <fstream>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

using namespace RDF;

TEST(RDFDatasetComparisonTest, areIsomorphic_withEmptyDatasets_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withSameSizeDatasets_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");
    dataset1.addTripleToGraph(oneName, one);
    dataset2.addTripleToGraph(oneName, one);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withDifferentSubjectsInDatasets_isFalse) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");
    RDFTriple two("http://example.com/subject2", "http://example.com/prop1", "1");
    dataset1.addTripleToGraph(oneName, one);
    dataset1.addTripleToGraph(oneName, two);
    dataset2.addTripleToGraph(oneName, one);
    dataset2.addTripleToGraph(oneName, two);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withSameSizeDatasetsWithSameSizeBlanks_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");
    RDFTriple oneBlank("_:b0", "http://example.com/prop1", "1");
    dataset1.addTripleToGraph(oneName, one);
    dataset2.addTripleToGraph(oneName, one);
    dataset1.addTripleToGraph(oneName, oneBlank);
    dataset2.addTripleToGraph(oneName, oneBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withSameSizeDatasetsWithSameSizeBlanksAndDifferentNames_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");
    RDFTriple oneBlank("_:b0", "http://example.com/prop1", "1");
    RDFTriple twoBlank("_:b1", "http://example.com/prop1", "1");
    dataset1.addTripleToGraph(oneName, one);
    dataset2.addTripleToGraph(oneName, one);
    dataset1.addTripleToGraph(oneName, oneBlank);
    dataset2.addTripleToGraph(oneName, twoBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withSameSizeBlanksAndDifferentNames_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple oneBlank("_:b0", "http://example.com/prop1", "1");
    RDFTriple twoBlank("_:b1", "http://example.com/prop1", "1");
    RDFTriple threeBlank("_:b5", "http://example.com/prop1", "1");
    RDFTriple fourBlank("_:b7", "http://example.com/prop1", "1");
    dataset1.addTripleToGraph(oneName, oneBlank);
    dataset1.addTripleToGraph(oneName, twoBlank);
    dataset2.addTripleToGraph(oneName, threeBlank);
    dataset2.addTripleToGraph(oneName, fourBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetComparisonTest, areIsomorphic_withBlankObjects_isTrue) {
    JsonLdOptions options;
    RDFDataset dataset1(options);
    RDFDataset dataset2(options);

    std::string oneName = "one";
    RDFTriple oneBlank("one", "http://example.com/prop1", "_:b1");
    RDFTriple twoBlank("one", "http://example.com/prop1", "_:b2");
    dataset1.addTripleToGraph(oneName, oneBlank);
    dataset2.addTripleToGraph(oneName, twoBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

