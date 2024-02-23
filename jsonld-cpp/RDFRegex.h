#ifndef LIBJSONLD_CPP_RDFREGEX_H
#define LIBJSONLD_CPP_RDFREGEX_H

#include <string>

namespace RDFRegex {
    // todo: remove note
    // Note: The following regexes for matching RDF data is not quite complete. Please see
    // https://www.w3.org/TR/n-quads/#sec-grammar for other items needed when unicode support
    // is completed.
    extern const std::string EOLN;
    extern const std::string WS;
    extern const std::string WSO;
    extern const std::string EMPTY;

    extern const std::string IRI;
    extern const std::string BNODE;
    extern const std::string PLAIN;
    extern const std::string DATATYPE;
    extern const std::string LANGUAGE;
    extern const std::string LITERAL;

    extern const std::string SUBJECT;
    extern const std::string PROPERTY;
    extern const std::string OBJECT;
    extern const std::string GRAPH;

    extern const std::string QUAD;

    extern const std::string HEX;
    extern const std::string UCHAR_MATCHED;


}

#endif //LIBJSONLD_CPP_RDFREGEX_H
