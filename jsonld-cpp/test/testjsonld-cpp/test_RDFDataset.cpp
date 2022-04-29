#include <jsonld-cpp/RDFDataset.h>
#include <jsonld-cpp/ObjUtils.h>

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

TEST(RDFDatasetTest, insert_one_quad_vector) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer;
    RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::vector<Quad> ones;
    std::string oneName = "one";
    ones.emplace_back("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    dataset.insert(std::make_pair(oneName, ones));

    std::vector<Quad> result = dataset.at(oneName);
    EXPECT_EQ(ones, result);
}

TEST(RDFDatasetTest, insert_two_quad_vectors) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer;
    RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::vector<Quad> ones;
    std::string oneName = "one";
    ones.emplace_back("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    dataset.insert(std::make_pair(oneName, ones));

    std::vector<Quad> twos;
    std::string twoName = "two";
    twos.emplace_back("http://example.com/subject2", "http://example.com/prop2", "2", &twoName);
    dataset.insert(std::make_pair(twoName, twos));

    std::vector<Quad> result = dataset.at(oneName);
    EXPECT_EQ(ones, result);

    result = dataset.at(twoName);
    EXPECT_EQ(twos, result);
}

TEST(RDFDatasetTest, add_one_quad_to_one_vector) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer;
    RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::string oneName = "one";

    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);

    dataset.addQuad(oneName, one);

    std::vector<Quad> result = dataset.at(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);
}

TEST(RDFDatasetTest, add_one_quad_twice_to_one_vector) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer;
    RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::string oneName = "one";

    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);

    dataset.addQuad(oneName, one);
    dataset.addQuad(oneName, one);

    std::vector<Quad> result = dataset.at(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);
}

TEST(RDFDatasetTest, add_two_quad_to_two_vectors) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer;
    RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::string oneName = "one";
    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    dataset.addQuad(oneName, one);

    std::string twoName = "two";
    Quad two("http://example.com/subject2", "http://example.com/prop2", "2", &twoName);
    dataset.addQuad(twoName, two);

    std::vector<Quad> result = dataset.at(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);

    std::vector<Quad> result2 = dataset.at(twoName);
    EXPECT_EQ(1, result2.size());
    EXPECT_EQ(two, result2[0]);
}

TEST(RDFDatasetTest, graphnames_canBeComparedEqual) {

    std::string name = "Name";
    RDF::RDFGraphName foo =
            RDF::RDFGraphName::createRDFGraphName(name, RDF::RDFGraphName::Type::DEFAULT);
    RDF::RDFGraphName bar =
            RDF::RDFGraphName::createRDFGraphName(name, RDF::RDFGraphName::Type::DEFAULT);

    EXPECT_EQ(foo, bar);

}

TEST(RDFDatasetTest, graphnames_canBeComparedRelational) {

    RDF::RDFGraphName foo =
            RDF::RDFGraphName::createRDFGraphName("b", RDF::RDFGraphName::Type::DEFAULT);
    RDF::RDFGraphName bar =
            RDF::RDFGraphName::createRDFGraphName("a", RDF::RDFGraphName::Type::DEFAULT);

    EXPECT_LT(bar, foo);
}


TEST(RDFDatasetTest, areIsomorphic_withEmptyDatasets_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withSameSizeDatasets_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    dataset1.addQuad(oneName, one);
    dataset2.addQuad(oneName, one);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withDifferentSubjectsInDatasets_isFalse) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    Quad two("http://example.com/subject2", "http://example.com/prop1", "1", &oneName);
    dataset1.addQuad(oneName, one);
    dataset1.addQuad(oneName, two);
    dataset2.addQuad(oneName, one);
    dataset2.addQuad(oneName, two);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withSameSizeDatasetsWithSameSizeBlanks_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    Quad oneBlank("_:b0", "http://example.com/prop1", "1", &oneName);
    dataset1.addQuad(oneName, one);
    dataset2.addQuad(oneName, one);
    dataset1.addQuad(oneName, oneBlank);
    dataset2.addQuad(oneName, oneBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withSameSizeDatasetsWithSameSizeBlanksAndDifferentNames_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad one("http://example.com/subject1", "http://example.com/prop1", "1", &oneName);
    Quad oneBlank("_:b0", "http://example.com/prop1", "1", &oneName);
    Quad twoBlank("_:b1", "http://example.com/prop1", "1", &oneName);
    dataset1.addQuad(oneName, one);
    dataset2.addQuad(oneName, one);
    dataset1.addQuad(oneName, oneBlank);
    dataset2.addQuad(oneName, twoBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withSameSizeBlanksAndDifferentNames_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad oneBlank("_:b0", "http://example.com/prop1", "1", &oneName);
    Quad twoBlank("_:b1", "http://example.com/prop1", "1", &oneName);
    Quad threeBlank("_:b5", "http://example.com/prop1", "1", &oneName);
    Quad fourBlank("_:b7", "http://example.com/prop1", "1", &oneName);
    dataset1.addQuad(oneName, oneBlank);
    dataset1.addQuad(oneName, twoBlank);
    dataset2.addQuad(oneName, threeBlank);
    dataset2.addQuad(oneName, fourBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}

TEST(RDFDatasetTest, areIsomorphic_withBlankObjects_isTrue) {
    JsonLdOptions options;
    UniqueNamer blankNodeUniqueNamer1;
    RDFDataset dataset1(options, &blankNodeUniqueNamer1);
    UniqueNamer blankNodeUniqueNamer2;
    RDFDataset dataset2(options, &blankNodeUniqueNamer2);

    std::string oneName = "one";
    Quad oneBlank("one", "http://example.com/prop1", "_:b1", &oneName);
    Quad twoBlank("one", "http://example.com/prop1", "_:b2", &oneName);
    dataset1.addQuad(oneName, oneBlank);
    dataset2.addQuad(oneName, twoBlank);

    EXPECT_TRUE(areIsomorphic(dataset1, dataset2));

}
