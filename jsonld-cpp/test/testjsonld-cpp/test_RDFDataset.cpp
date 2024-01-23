#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/RDFDataset.h>

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

TEST(RDFDatasetTest, add_one_triple_to_one_graph) {
    // create dataset, add one triple to it under a graph name. request the graph by name and
    // verify it has one member.
    JsonLdOptions options;
    RDFDataset dataset(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");

    dataset.addTripleToGraph(oneName, one);

    RDFGraph result = dataset.getGraph(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);
}

TEST(RDFDatasetTest, add_one_triple_twice_to_one_graph) {
    // create dataset, add one triple to it under a graph name. add it again. request the
    // graph by name and verify it has one member.
    JsonLdOptions options;
    RDFDataset dataset(options);

    std::string oneName = "one";

    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");

    dataset.addTripleToGraph(oneName, one);
    dataset.addTripleToGraph(oneName, one);

    RDFGraph result = dataset.getGraph(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);
}

TEST(RDFDatasetTest, add_two_triples_to_two_graphs) {
    // create dataset, add one triple to it under a graph name. add another triple to it
    // under another graph name. request the graphs by name and verify they have one member.
    JsonLdOptions options;
    RDFDataset dataset(options);

    std::string oneName = "one";
    RDFTriple one("http://example.com/subject1", "http://example.com/prop1", "1");
    dataset.addTripleToGraph(oneName, one);

    std::string twoName = "two";
    RDFTriple two("http://example.com/subject2", "http://example.com/prop2", "2");
    dataset.addTripleToGraph(twoName, two);

    RDFGraph result = dataset.getGraph(oneName);
    EXPECT_EQ(1, result.size());
    EXPECT_EQ(one, result[0]);

    RDFGraph result2 = dataset.getGraph(twoName);
    EXPECT_EQ(1, result2.size());
    EXPECT_EQ(two, result2[0]);
}

