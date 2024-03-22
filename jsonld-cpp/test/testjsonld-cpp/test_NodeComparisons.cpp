#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/RDFDataset.h>
#include <jsonld-cpp/RDFQuad.h>

#include <gtest/gtest.h>

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

    RDFQuad q1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", nullptr);

    RDFQuad q2("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", nullptr);

    EXPECT_EQ(q1, q2);
}

TEST(NodeComparisonsTest, quads_withSameGraphs_areEqual) {

    std::string graph = "foo";

    RDFQuad q1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", &graph);

    RDFQuad q2("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", &graph);

    EXPECT_EQ(q1, q2);
}

TEST(NodeComparisonsTest, quads_withDifferentSubjects_areNotEqual) {

    RDFQuad q1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", nullptr);

    RDFQuad q2_diffSubject("https://example.com/s2", "https://example.com/p1",
                           "https://example.com/o1", nullptr);

    EXPECT_NE(q1, q2_diffSubject);
}

TEST(NodeComparisonsTest, quads_withDifferentPredicates_areNotEqual) {

    RDFQuad q1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", nullptr);

    RDFQuad q2_diffPredicate("https://example.com/s1", "https://example.com/p2",
                             "https://example.com/o1", nullptr);

    EXPECT_NE(q1, q2_diffPredicate);
}

TEST(NodeComparisonsTest, quads_withDifferentObjects_areNotEqual) {

    RDFQuad q1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1", nullptr);

    RDFQuad q2_diffObject("https://example.com/s1", "https://example.com/p1",
                          "https://example.com/o2", nullptr);

    EXPECT_NE(q1, q2_diffObject);
}

TEST(NodeComparisonsTest, quads_withDifferentGraphs_sortCorrectly) {

    std::string graph1 = "aaa";
    std::string graph2 = "aab";

    std::set<std::shared_ptr<RDFQuad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<RDFQuad>("https://example.com/s1", "https://example.com/p1",
                                           "https://example.com/o1", &graph2));

    quads.insert(std::make_shared<RDFQuad>("https://example.com/s1", "https://example.com/p1",
                                           "https://example.com/o1", &graph1));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getGraph()->getValue(), graph1);
    ci++;
    EXPECT_EQ((*ci)->getGraph()->getValue(), graph2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentSubjects_sortCorrectly) {

    std::string subject1 = "https://example.com/s1";
    std::string subject2 = "https://example.com/s2";

    std::set<std::shared_ptr<RDFQuad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<RDFQuad>(subject2, "https://example.com/p1",
                                           "https://example.com/o1", nullptr));

    quads.insert(std::make_shared<RDFQuad>(subject1, "https://example.com/p1",
                                           "https://example.com/o1", nullptr));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getSubject()->getValue(), subject1);
    ci++;
    EXPECT_EQ((*ci)->getSubject()->getValue(), subject2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentPredicates_sortCorrectly) {

    std::string predicate1 = "https://example.com/p1";
    std::string predicate2 = "https://example.com/p2";

    std::set<std::shared_ptr<RDFQuad>, QuadPtrLess> quads;

    std::shared_ptr<RDFQuad> q1 = std::make_shared<RDFQuad>("https://example.com/s1", predicate2,
                                                            "https://example.com/o1", nullptr);
    quads.insert(q1);

    std::shared_ptr<RDFQuad> q2 = std::make_shared<RDFQuad>("https://example.com/s1", predicate1,
                                                            "https://example.com/o1", nullptr);
    quads.insert(q2);

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getPredicate()->getValue(), predicate1);
    ci++;
    EXPECT_EQ((*ci)->getPredicate()->getValue(), predicate2);
    ci++;

}

TEST(NodeComparisonsTest, quads_withDifferentObjects_sortCorrectly) {

    std::string object1 = "https://example.com/o1";
    std::string object2 = "https://example.com/o2";

    std::set<std::shared_ptr<RDFQuad>, QuadPtrLess> quads;

    quads.insert(std::make_shared<RDFQuad>("https://example.com/s1", "https://example.com/p1",
                                           object2, nullptr));

    quads.insert(std::make_shared<RDFQuad>("https://example.com/s1", "https://example.com/p1",
                                           object1, nullptr));

    auto ci = quads.begin();

    EXPECT_EQ((*ci)->getObject()->getValue(), object1);
    ci++;
    EXPECT_EQ((*ci)->getObject()->getValue(), object2);
    ci++;

}

TEST(NodeComparisonsTest, triples_areEqual) {

    RDFTriple t1("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1");

    RDFTriple t2("https://example.com/s1", "https://example.com/p1",
               "https://example.com/o1");

    EXPECT_EQ(t1, t2);
}

TEST(NodeComparisonsTest, triples_withDifferentSubjects_areNotEqual) {

    RDFTriple t1("https://example.com/s1", "https://example.com/p1",
                 "https://example.com/o1");

    RDFTriple t2_diffSubject("https://example.com/s2", "https://example.com/p1",
                 "https://example.com/o1");

    EXPECT_NE(t1, t2_diffSubject);
}

TEST(NodeComparisonsTest, triples_withDifferentPredicates_areNotEqual) {

    RDFTriple t1("https://example.com/s1", "https://example.com/p1",
                 "https://example.com/o1");

    RDFTriple t2_diffPredicate("https://example.com/s1", "https://example.com/p2",
                             "https://example.com/o1");

    EXPECT_NE(t1, t2_diffPredicate);
}

TEST(NodeComparisonsTest, triples_withDifferentObjects_areNotEqual) {

    RDFTriple t1("https://example.com/s1", "https://example.com/p1",
                 "https://example.com/o1");

    RDFTriple t2_diffObject("https://example.com/s1", "https://example.com/p1",
                             "https://example.com/o2");

    EXPECT_NE(t1, t2_diffObject);
}

TEST(NodeComparisonsTest, triples_withLiteralsAndDataTypes_madeFromSamePointers_areEqual) {

    std::shared_ptr<RDF::Node> subject, predicate, object;

    subject = std::make_shared<IRI>("https://example.com/s1");
    predicate = std::make_shared<IRI>("https://example.com/p1");
    std::string dataType = "https://www.w3.org/2001/XMLSchema#boolean";
    object = std::make_shared<Literal>("true", &dataType);

    RDFTriple t1(subject, predicate,object);

    RDFTriple t2(subject, predicate,object);

    EXPECT_EQ(t1, t2);
}

TEST(NodeComparisonsTest, triples_withLiteralsAndDataTypes_madeFromDifferentPointers_areEqual) {

    std::shared_ptr<RDF::Node> subject1, predicate1, object1;

    subject1 = std::make_shared<IRI>("https://example.com/s1");
    predicate1 = std::make_shared<IRI>("https://example.com/p1");
    std::string dataType = "https://www.w3.org/2001/XMLSchema#boolean";
    object1 = std::make_shared<Literal>("true", &dataType);

    std::shared_ptr<RDF::Node> subject2, predicate2, object2;

    subject2 = std::make_shared<IRI>("https://example.com/s1");
    predicate2 = std::make_shared<IRI>("https://example.com/p1");
    object2 = std::make_shared<Literal>("true", &dataType);

    RDFTriple t1(subject1, predicate1,object1);

    RDFTriple t2(subject2, predicate2,object2);

    EXPECT_EQ(t1, t2);
}

