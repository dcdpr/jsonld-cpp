#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/RDFTriple.h>

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

TEST(RDFTripleTest, assignment) {
    // create two RDFTriples, and assign one to the other.

    RDFTriple q1("s1", "p1", "o1");
    RDFTriple q2("s2", "p2", "o2");

    EXPECT_NE(q1, q2);

    q2 = q1;

    EXPECT_EQ(q1, q2);
}

TEST(RDFTripleTest, copy_constructor) {
    // create an RDFTriple, and create a second by copying the first.

    RDFTriple q1("s1", "p1", "o1");
    RDFTriple q2{q1};

    EXPECT_EQ(q1, q2);
}

TEST(RDFTripleTest, move_constructor) {
    // create an RDFTriple, and create a second by moving the first.

    RDFTriple q1("s1", "p1", "o1");

    auto s = q1.getSubject();
    auto p = q1.getPredicate();
    auto o = q1.getObject();

    RDFTriple q2 = std::move(q1);

    EXPECT_EQ(q1.getSubject(), nullptr);
    EXPECT_EQ(q1.getPredicate(), nullptr);
    EXPECT_EQ(q1.getObject(), nullptr);
    EXPECT_EQ(q2.getSubject(), s);
    EXPECT_EQ(q2.getPredicate(), p);
    EXPECT_EQ(q2.getObject(), o);
}

TEST(RDFTripleTest, move_assignment) {
    // create two RDFTriples, then replace the second by move-assigning the first.

    RDFTriple q1("s1", "p1", "o1");

    auto s = q1.getSubject();
    auto p = q1.getPredicate();
    auto o = q1.getObject();

    RDFTriple q2("s2", "p2", "o2");

    q2 = std::move(q1);

    EXPECT_EQ(q1.getSubject(), nullptr);
    EXPECT_EQ(q1.getPredicate(), nullptr);
    EXPECT_EQ(q1.getObject(), nullptr);
    EXPECT_EQ(q2.getSubject(), s);
    EXPECT_EQ(q2.getPredicate(), p);
    EXPECT_EQ(q2.getObject(), o);
}

