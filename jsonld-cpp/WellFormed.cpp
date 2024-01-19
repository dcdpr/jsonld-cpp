#include <regex>
#include "WellFormed.h"
#include "JsonLdUtils.h"
#include "BlankNodeNames.h"
#include "RDFRegex.h"

bool WellFormed::iri(const std::string &str) {
    return JsonLdUtils::isAbsoluteIri(str);
}

bool WellFormed::blankNodeIdentifier(const std::string &str) {
    std::regex re(RDFRegex::BNODE);
    std::smatch match;
    return std::regex_match(str, match, re);
}

/**
 * Check if str is a well-formed language.
 *
 *
 * @param str
 * @return
 */
bool WellFormed::language(const std::string &str) {
    // The JSON-LD spec mentions in several places that a "language is
    // well-formed according to section 2.2.9 of [BCP47]" and BCP47 contains
    // an extensive grammar for defining a well-formed language tag. For
    // now, however, we will use a much simpler definition for a language
    // tag given by the "RDF 1.1 N-Quads" spec:
    // https://www.w3.org/TR/n-quads/#sec-grammar
    std::regex re(RDFRegex::LANGUAGE);
    std::smatch match;
    return std::regex_match(str, match, re);
}


bool WellFormed::literal(const std::string &str) {
    std::regex re(RDFRegex::LITERAL);
    std::smatch match;
    return std::regex_match(str, match, re);
}

bool WellFormed::rdf_subject(const std::string &str) {
    return iri(str) || blankNodeIdentifier(str);
}

bool WellFormed::rdf_predicate(const std::string &str) {
    return iri(str);
}

bool WellFormed::rdf_object(const std::string &str) {
    return iri(str) || blankNodeIdentifier(str) || literal(str);
}

bool WellFormed::rdf_graph_name(const std::string &str) {
    return iri(str) || blankNodeIdentifier(str);
}


