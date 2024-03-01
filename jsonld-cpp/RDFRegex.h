#ifndef LIBJSONLD_CPP_RDFREGEX_H
#define LIBJSONLD_CPP_RDFREGEX_H

#include <string>

namespace RDFRegex {

    extern const std::string EOLN;
    extern const std::string EMPTY;

    extern const std::string BNODE;
    extern const std::string LANGUAGE;
    extern const std::string LITERAL;

    extern const std::string SUBJECT;
    extern const std::string PREDICATE;
    extern const std::string OBJECT;
    extern const std::string GRAPH;

    extern const std::string QUAD;

    extern const int QUAD_SUBJECT_AS_IRI;
    extern const int QUAD_SUBJECT_AS_BNODE;
    extern const int QUAD_PREDICATE;
    extern const int QUAD_OBJECT_AS_IRI;
    extern const int QUAD_OBJECT_AS_BNODE;
    extern const int QUAD_OBJECT_AS_LITERAL;
    extern const int QUAD_OBJECT_AS_LITERAL_DATATYPE;
    extern const int QUAD_OBJECT_AS_LITERAL_LANGUAGETAG;
    extern const int QUAD_GRAPH_AS_IRI;
    extern const int QUAD_GRAPH_AS_BNODE;

    extern const std::string UNICODE_CODEPOINT;

    extern const int UNICODE_CONTROL_CHARS;
    extern const int UNICODE_BASIC_MULTILINGUAL_PLANE;
    extern const int UNICODE_HIGHER_PLANE;

}

#endif //LIBJSONLD_CPP_RDFREGEX_H
