#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/RDFQuad.h>

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

TEST(RDFQuadTest, assignment) {
    // create two RDFQuads, and assign one to the other.

    RDFQuad q1("s1", "p1", "o1", nullptr);
    RDFQuad q2("s2", "p2", "o2", nullptr);

    EXPECT_NE(q1, q2);

    q2 = q1;

    EXPECT_EQ(q1, q2);
}

TEST(RDFQuadTest, copy_constructor) {
    // create an RDFQuad, and create a second by copying the first.

    RDFQuad q1("s1", "p1", "o1", nullptr);
    RDFQuad q2{q1};

    EXPECT_EQ(q1, q2);
}

TEST(RDFQuadTest, move_constructor) {
    // create an RDFQuad, and create a second by moving the first.

    RDFQuad q1("s1", "p1", "o1", nullptr);

    auto s = q1.getSubject();
    auto p = q1.getPredicate();
    auto o = q1.getObject();
    auto g = q1.getGraph();

    RDFQuad q2 = std::move(q1);

    EXPECT_EQ(q1.getSubject(), nullptr);
    EXPECT_EQ(q1.getPredicate(), nullptr);
    EXPECT_EQ(q1.getObject(), nullptr);
    EXPECT_EQ(q1.getGraph(), nullptr);
    EXPECT_EQ(q2.getSubject(), s);
    EXPECT_EQ(q2.getPredicate(), p);
    EXPECT_EQ(q2.getObject(), o);
    EXPECT_EQ(q2.getGraph(), g);
}

TEST(RDFQuadTest, move_assignment) {
    // create two RDFQuads, then replace the second by move-assigning the first.

    RDFQuad q1("s1", "p1", "o1", nullptr);

    auto s = q1.getSubject();
    auto p = q1.getPredicate();
    auto o = q1.getObject();
    auto g = q1.getGraph();

    RDFQuad q2("s2", "p2", "o2", nullptr);

    q2 = std::move(q1);

    EXPECT_EQ(q1.getSubject(), nullptr);
    EXPECT_EQ(q1.getPredicate(), nullptr);
    EXPECT_EQ(q1.getObject(), nullptr);
    EXPECT_EQ(q1.getGraph(), nullptr);
    EXPECT_EQ(q2.getSubject(), s);
    EXPECT_EQ(q2.getPredicate(), p);
    EXPECT_EQ(q2.getObject(), o);
    EXPECT_EQ(q2.getGraph(), g);
}

