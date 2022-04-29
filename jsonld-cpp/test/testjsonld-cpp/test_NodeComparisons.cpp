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

TEST(NodeComparisonsTest, literals_withSameValue_areEqual) {
    Literal l1("Same", nullptr, nullptr);
    Literal l2("Same", nullptr, nullptr);
    EXPECT_EQ(l1, l2);
}

TEST(NodeComparisonsTest, literals_withDifferentValue_areNotEqual) {
    Literal l1("Same", nullptr, nullptr);
    Literal l2("Not Same", nullptr, nullptr);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueSameLang_areEqual) {
    std::string lang_en = "en";
    Literal l1("Same", nullptr, &lang_en);
    Literal l2("Same", nullptr, &lang_en);
    EXPECT_EQ(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueNullLang_areNotEqual) {
    std::string lang_en = "en";
    Literal l1("Same", nullptr, &lang_en);
    Literal l2("Same", nullptr, nullptr);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueDifferentLang_areNotEqual) {
    std::string lang_en = "en";
    std::string lang_no = "no";
    Literal l1("Same", nullptr, &lang_en);
    Literal l2("Same", nullptr, &lang_no);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, literals_withDifferentValueSameLang_areNotEqual) {
    std::string lang_en = "en";
    Literal l1("Same", nullptr, &lang_en);
    Literal l2("Not Same", nullptr, &lang_en);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueSameType_areEqual) {
    std::string dt_integer = JsonLdConsts::XSD_INTEGER;
    Literal l1("Same", &dt_integer, nullptr);
    Literal l2("Same", &dt_integer, nullptr);
    EXPECT_EQ(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueNullType_areNotEqual) {
    std::string dt_integer = JsonLdConsts::XSD_INTEGER;
    Literal l1("Same", &dt_integer, nullptr);
    Literal l2("Same", nullptr, nullptr);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, literals_withSameValueDifferentType_areNotEqual) {
    std::string dt_integer = JsonLdConsts::XSD_INTEGER;
    std::string dt_string = JsonLdConsts::XSD_STRING;
    Literal l1("Same", &dt_integer, nullptr);
    Literal l2("Same", &dt_string, nullptr);
    EXPECT_NE(l1, l2);
}

TEST(NodeComparisonsTest, iris_withSameIri_areEqual) {
    IRI i1("https://www.example.com");
    IRI i2("https://www.example.com");
    EXPECT_EQ(i1, i2);
}

TEST(NodeComparisonsTest, iris_withDifferentIri_areNotEqual) {
    IRI i1("https://www.example.com");
    IRI i2("https://www.example.com/other");
    EXPECT_NE(i1, i2);
}

TEST(NodeComparisonsTest, literals_differentThanIris) {
    std::string dt_string = JsonLdConsts::XSD_STRING;
    IRI i1("Same");
    Literal l1("Same", &dt_string, nullptr);
    EXPECT_NE(i1, l1);
}

TEST(NodeComparisonsTest, literals_differentThanBlankNodes) {
    std::string dt_string = JsonLdConsts::XSD_STRING;
    RDF::BlankNode b1("Same");
    Literal l1("Same", &dt_string, nullptr);
    EXPECT_NE(b1, l1);
}

TEST(NodeComparisonsTest, iris_differentThanBlankNodes) {
    IRI i1("Same");
    RDF::BlankNode b1("Same");
    EXPECT_NE(i1, b1);
}

TEST(NodeComparisonsTest, sortingIris_differentValues) {

    std::set<std::shared_ptr<Node>, NodePtrLess> iris;

    std::string iriStr1 = "https://a.example.com";
    std::string iriStr2 = "https://b.example.com";
    std::string iriStr3 = "https://c.example.com";

    iris.insert(std::make_shared<IRI>(iriStr2));
    iris.insert(std::make_shared<IRI>(iriStr3));
    iris.insert(std::make_shared<IRI>(iriStr1));

    auto ci = iris.begin();

    EXPECT_EQ((*ci)->getValue(), iriStr1);
    ci++;
    EXPECT_EQ((*ci)->getValue(), iriStr2);
    ci++;
    EXPECT_EQ((*ci)->getValue(), iriStr3);
}

TEST(NodeComparisonsTest, sortingBlankNodes_differentValues) {

    std::set<std::shared_ptr<Node>, NodePtrLess> bns;

    std::string bnStr1 = "bn1";
    std::string bnStr2 = "bn2";
    std::string bnStr3 = "bn3";

    bns.insert(std::make_shared<RDF::BlankNode>(bnStr2));
    bns.insert(std::make_shared<RDF::BlankNode>(bnStr3));
    bns.insert(std::make_shared<RDF::BlankNode>(bnStr1));

    auto ci = bns.begin();

    EXPECT_EQ((*ci)->getValue(), bnStr1);
    ci++;
    EXPECT_EQ((*ci)->getValue(), bnStr2);
    ci++;
    EXPECT_EQ((*ci)->getValue(), bnStr3);
}

TEST(NodeComparisonsTest, sortingLiterals_differentValues) {

    std::set<std::shared_ptr<Node>, NodePtrLess> literals;

    std::string valStr1 = "abe";
    std::string valStr2 = "barbara";
    std::string valStr3 = "catherine";

    literals.insert(std::make_shared<Literal>(valStr2));
    literals.insert(std::make_shared<Literal>(valStr3));
    literals.insert(std::make_shared<Literal>(valStr1));

    auto ci = literals.begin();

    EXPECT_EQ((*ci)->getValue(), valStr1);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr2);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr3);
}

TEST(NodeComparisonsTest, sortingLiterals_sameValues_differentDatatypes) {

    std::set<std::shared_ptr<Node>, NodePtrLess> literals;

    std::string valStr1 = "1";
    std::string dataTypeStr1 = JsonLdConsts::XSD_BOOLEAN;
    std::string valStr2 = "1";
    std::string dataTypeStr2 = JsonLdConsts::XSD_INTEGER;
    std::string valStr3 = "1";
    std::string dataTypeStr3 = JsonLdConsts::XSD_STRING;

    literals.insert(std::make_shared<Literal>(valStr2, &dataTypeStr2));
    literals.insert(std::make_shared<Literal>(valStr3, &dataTypeStr3));
    literals.insert(std::make_shared<Literal>(valStr1, &dataTypeStr1));

    auto ci = literals.begin();

    EXPECT_EQ((*ci)->getValue(), valStr1);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr2);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr3);
}

TEST(NodeComparisonsTest, sortingLiterals_sameValues_differentLanguages) {

    std::set<std::shared_ptr<Node>, NodePtrLess> literals;

    std::string valStr1 = "abe";
    std::string langStr1 = "de";
    std::string valStr2 = "abe";
    std::string langStr2 = "en";
    std::string valStr3 = "abe";
    std::string langStr3 = "fr";

    literals.insert(std::make_shared<Literal>(valStr2, nullptr, &langStr2));
    literals.insert(std::make_shared<Literal>(valStr3, nullptr, &langStr3));
    literals.insert(std::make_shared<Literal>(valStr1, nullptr, &langStr1));

    auto ci = literals.begin();

    EXPECT_EQ((*ci)->getValue(), valStr1);
    EXPECT_EQ((*ci)->getLanguage(), langStr1);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr2);
    EXPECT_EQ((*ci)->getLanguage(), langStr2);
    ci++;
    EXPECT_EQ((*ci)->getValue(), valStr3);
    EXPECT_EQ((*ci)->getLanguage(), langStr3);
}

TEST(NodeComparisonsTest, quads_withNullGraphs_areEqual) {

    Quad q1("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1",nullptr);

    Quad q2("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1", nullptr);

    EXPECT_EQ(q1, q2);
}

TEST(NodeComparisonsTest, quads_withSameGraphs_areEqual) {

    std::string graph = "foo";

    Quad q1("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1", &graph);

    Quad q2("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1", &graph);

    EXPECT_EQ(q1, q2);
}

TEST(NodeComparisonsTest, quads_withDifferentSubjects_areNotEqual) {

    Quad q1("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1",nullptr);

    Quad q2_diffSubject("http://example.com/s2", "http://example.com/p1",
                        "http://example.com/o1", nullptr);

    EXPECT_NE(q1, q2_diffSubject);
}

TEST(NodeComparisonsTest, quads_withDifferentPredicates_areNotEqual) {

    Quad q1("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1",nullptr);

    Quad q2_diffPredicate("http://example.com/s1", "http://example.com/p2",
                         "http://example.com/o1", nullptr);

    EXPECT_NE(q1, q2_diffPredicate);
}

TEST(NodeComparisonsTest, quads_withDifferentObjects_areNotEqual) {

    Quad q1("http://example.com/s1", "http://example.com/p1",
            "http://example.com/o1",nullptr);

    Quad q2_diffObject("http://example.com/s1", "http://example.com/p1",
                       "http://example.com/o2", nullptr);

    EXPECT_NE(q1, q2_diffObject);
}

TEST(NodeComparisonsTest, quads_withDifferentGraphs_sortCorrectly) {

    std::string graph1 = "aaa";
    std::string graph2 = "aab";

    std::set<std::shared_ptr<Quad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<Quad>("http://example.com/s1", "http://example.com/p1",
                                        "http://example.com/o1", &graph2));

    quads.insert(std::make_shared<Quad>("http://example.com/s1", "http://example.com/p1",
                                        "http://example.com/o1", &graph1));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getGraph()->getValue(), graph1);
    ci++;
    EXPECT_EQ((*ci)->getGraph()->getValue(), graph2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentSubjects_sortCorrectly) {

    std::string subject1 = "http://example.com/s1";
    std::string subject2 = "http://example.com/s2";

    std::set<std::shared_ptr<Quad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<Quad>(subject2, "http://example.com/p1",
                                        "http://example.com/o1", nullptr));

    quads.insert(std::make_shared<Quad>(subject1, "http://example.com/p1",
                                        "http://example.com/o1", nullptr));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getSubject()->getValue(), subject1);
    ci++;
    EXPECT_EQ((*ci)->getSubject()->getValue(), subject2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentPredicates_sortCorrectly) {

    std::string predicate1 = "http://example.com/p1";
    std::string predicate2 = "http://example.com/p2";

    std::set<std::shared_ptr<Quad>, QuadPtrLess> quads;

    std::shared_ptr<Quad> q1 = std::make_shared<Quad>("http://example.com/s1", predicate2,
                                        "http://example.com/o1", nullptr);
    quads.insert(q1);

    std::shared_ptr<Quad> q2 = std::make_shared<Quad>("http://example.com/s1", predicate1,
                                        "http://example.com/o1", nullptr);
    quads.insert(q2);

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getPredicate()->getValue(), predicate1);
    ci++;
    EXPECT_EQ((*ci)->getPredicate()->getValue(), predicate2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentObjects_sortCorrectly) {

    std::string object1 = "http://example.com/o1";
    std::string object2 = "http://example.com/o2";

    std::set<std::shared_ptr<Quad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<Quad>("http://example.com/s1", "http://example.com/p1",
                                        object2, nullptr));

    quads.insert(std::make_shared<Quad>("http://example.com/s1", "http://example.com/p1",
                                        object1, nullptr));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getObject()->getValue(), object1);
    ci++;
    EXPECT_EQ((*ci)->getObject()->getValue(), object2);
    ci++;

}

