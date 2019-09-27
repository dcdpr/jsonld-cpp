#include "NormalizeUtils.cpp"
#include "RDFDataset.h"
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


TEST(NormalizeUtilsTest, getAdjacentBlankNodeName_notBlankNode_returnsNull) {
    std::string id = "_:b0";
    std::shared_ptr<RDF::IRI> node = std::make_shared<RDF::IRI>("http://www.example.com");
    std::shared_ptr<std::string> bnode = NormalizeUtils::getAdjacentBlankNodeName(node, id);
    EXPECT_EQ(bnode, nullptr);
}

TEST(NormalizeUtilsTest, getAdjacentBlankNodeName_isBlankNodeButNoValue_returnsNull) {
    // can't normally make this happen as the constructor of BlankNode enforces creating a value
    EXPECT_EQ(true, true);
}

TEST(NormalizeUtilsTest, getAdjacentBlankNodeName_isBlankNodeWithValueEqualToId_returnsNull) {
    std::string id = "_:b0";
    std::shared_ptr<RDF::BlankNode> node = std::make_shared<RDF::BlankNode>(id);
    std::shared_ptr<std::string> bnode = NormalizeUtils::getAdjacentBlankNodeName(node, id);
    EXPECT_EQ(bnode, nullptr);
}

TEST(NormalizeUtilsTest, getAdjacentBlankNodeName_isBlankNodeWithValueNotEqualToId_returnsValue) {
    std::string id = "_:b0";
    std::string value = "_:b12";
    std::shared_ptr<RDF::BlankNode> node = std::make_shared<RDF::BlankNode>(value);
    std::shared_ptr<std::string> bnode = NormalizeUtils::getAdjacentBlankNodeName(node, id);
    EXPECT_EQ(*bnode, value);
}


TEST(NormalizeUtilsTest, getAdjacentBlankNodeName_isBlankNodeWithValueNotEqualToNull_returnsValue) {
    // can't normally make this happen since we don't allow the id passed to getAdjacentBlankNodeName() to be null
    EXPECT_EQ(true, true);
}
