#include "RDFDataset.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <fstream>
#include <ObjUtils.h>

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

