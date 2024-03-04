#include "jsonld-cpp/RDFRegex.h"

namespace {

    const std::string WS(R"([ \t]+)");
    const std::string WSO(R"([ \t]*)");

    const std::string IRI(R"((?:<([^>]*)>))");

    const std::string PLAIN(R"lit("([^"\\]*(?:\\.[^"\\]*)*)")lit");
    const std::string DATATYPE("(?:\\^\\^" + IRI + ")");
    const std::string LANGUAGETAG("(?:@([a-z]+(?:-[a-zA-Z0-9]+)*))");

    const std::string HEX("[0-9A-Fa-f]");

}

namespace RDFRegex {

    const std::string EOLN(R"((?:\r\n)|(?:\n)|(?:\r))");
    const std::string EMPTY("^" + WSO + "$");

    const std::string BNODE(R"((_:(?:[A-Za-z_:0-9])(?:(?:[A-Za-z_:0-9\.-])*[A-Za-z_:0-9-])?))");
    const std::string LANGUAGE("([a-z]+(?:-[a-zA-Z0-9]+)*)");
    const std::string LITERAL("(?:" + PLAIN + "(?:" + DATATYPE + "|" + LANGUAGETAG + ")?)");

    const std::string SUBJECT("(?:" + IRI + "|" + BNODE + ")" + WS);
    const std::string PREDICATE(IRI + WS);
    const std::string OBJECT("(?:" + IRI + "|" + BNODE + "|" + LITERAL + ")" + WSO);
    const std::string GRAPH("(?:\\.|(?:(?:" + IRI + "|" + BNODE + ")" + WSO + "\\.))");

    const std::string QUAD("^" + WSO + SUBJECT + PREDICATE + OBJECT + GRAPH + WSO + "$");

    // These indexes represent the 'capturing group' within the large QUAD regex above
    const std::size_t QUAD_SUBJECT_AS_IRI = 1;
    const std::size_t QUAD_SUBJECT_AS_BNODE = 2;
    const std::size_t QUAD_PREDICATE = 3;
    const std::size_t QUAD_OBJECT_AS_IRI = 4;
    const std::size_t QUAD_OBJECT_AS_BNODE = 5;
    const std::size_t QUAD_OBJECT_AS_LITERAL = 6;
    const std::size_t QUAD_OBJECT_AS_LITERAL_DATATYPE = 7;
    const std::size_t QUAD_OBJECT_AS_LITERAL_LANGUAGETAG = 8;
    const std::size_t QUAD_GRAPH_AS_IRI = 9;
    const std::size_t QUAD_GRAPH_AS_BNODE = 10;

    const std::string UNICODE_CODEPOINT(R"(\u005C(?:([tbnrf\\\"'])|(?:u()" + HEX + "{4}))|(?:U(" + HEX + "{8})))");

    // These indexes represent the 'capturing group' within the UNICODE_CODEPOINT regex above
    const std::size_t UNICODE_CONTROL_CHARS = 1;
    const std::size_t UNICODE_BASIC_MULTILINGUAL_PLANE = 2;
    const std::size_t UNICODE_HIGHER_PLANE = 3;

}

