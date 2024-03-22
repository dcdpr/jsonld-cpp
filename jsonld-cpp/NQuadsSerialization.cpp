#include "jsonld-cpp/NQuadsSerialization.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <regex>
#include <iostream>

#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/detail/RDFRegex.h"
#include "jsonld-cpp/JsonLdError.h"


using namespace RDFRegex;

namespace {

    // return vector of strings after splitting input string into lines
    std::vector<std::string> splitLines(std::string &input) {
        std::vector<std::string> lines;
        std::regex rgx(EOLN);
        std::sregex_token_iterator i(input.begin(), input.end(), rgx, -1);
        std::sregex_token_iterator end;

        for (; i != end; ++i)
            lines.push_back(*i);
        return lines;
    }

    /**
     * Escapes input string, writes output to given stringstream
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    void escape(const std::string &str, std::stringstream &ss) {
        for (char c : str) {
            if (c < 0x8 ||
                (c >= 0xE && c <= 0x1F)) {
                ss << "\\u";
                ss << std::setfill('0') << std::setw(4) << c;
            } else {
                switch (c) {
                    case '\b': // 0x8
                        ss << "\\b";
                        break;
                    case '\t': // 0x9
                        ss << "\\t";
                        break;
                    case '\n': // 0xA
                        ss << "\\n";
                        break;
                    case '\v': // 0xB
                        ss << "\\v";
                        break;
                    case '\f': // 0xC
                        ss << "\\f";
                        break;
                    case '\r': // 0xD
                        ss << "\\r";
                        break;
                    case '\"':
                        ss << "\\\"";
                        break;
                    case '\'':
                        ss << "\\\'";
                        break;
                    case '\\':
                        ss << "\\\\";
                        break;
                    default:
                        ss << c;
                        break;
                }
            }
        }
    }

    void extractUnicodeCodepoint(const std::smatch &match, std::string &u) {
        std::string hex = match[UNICODE_BASIC_MULTILINGUAL_PLANE].matched ?
                          match[UNICODE_BASIC_MULTILINGUAL_PLANE].str() :
                          match[UNICODE_HIGHER_PLANE].str();
        long v = std::stol(hex, nullptr, 16);

        auto it = std::back_inserter(u);
        if (v < 0x80)                        // one octet
            *(it++) = static_cast<char>(v);
        else if (v < 0x800) {                // two octets
            *(it++) = static_cast<char>((v >> 6) | 0xc0);
            *(it++) = static_cast<char>((v & 0x3f) | 0x80);
        } else if (v < 0x10000) {            // three octets
            *(it++) = static_cast<char>((v >> 12) | 0xe0);
            *(it++) = static_cast<char>(((v >> 6) & 0x3f) | 0x80);
            *(it++) = static_cast<char>((v & 0x3f) | 0x80);
        } else {                             // four octets
            *(it++) = static_cast<char>((v >> 18) | 0xf0);
            *(it++) = static_cast<char>(((v >> 12) & 0x3f) | 0x80);
            *(it++) = static_cast<char>(((v >> 6) & 0x3f) | 0x80);
            *(it++) = static_cast<char>((v & 0x3f) | 0x80);
        }
    }

    bool extractControlCharacter(const std::smatch &match, std::string &u) {
        char c = match[UNICODE_CONTROL_CHARS].str()[0];
        switch (c) {
            case 'b':
                u = "\b";
                break;
            case 't':
                u = "\t";
                break;
            case 'n':
                u = "\n";
                break;
            case 'f':
                u = "\f";
                break;
            case 'r':
                u = "\r";
                break;
            case '\'':
                u = "'";
                break;
            case '\"':
                u = "\"";
                break;
            case '\\':
                u = '\\';
                break;
            default:
                return false;
        }
        return true;
    }

    /**
     * Un-escapes input string, writes output to given stringstream
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    void unescape(const std::string &str, std::stringstream &ss) {

        if(str.empty())
            return;

        std::regex charsRgx(UNICODE_CODEPOINT);
        auto chars_begin = std::sregex_iterator(str.begin(), str.end(), charsRgx);
        auto chars_end = std::sregex_iterator();

        if(chars_begin == chars_end) {
            ss << str;
            return;
        }

        std::string unescaped_result;
        auto out = std::back_inserter(unescaped_result);
        std::smatch last_match;

        for (std::sregex_iterator reg_it = chars_begin; reg_it != chars_end; ++reg_it) {
            const std::smatch &match = *reg_it;

            out = std::copy(match.prefix().first, match.prefix().second, out);

            std::string u;
            if (!match[UNICODE_CONTROL_CHARS].matched) {
                extractUnicodeCodepoint(match, u);
            } else {
                if(!extractControlCharacter(match, u))
                {
                    last_match = match;
                    continue;
                }
            }

            out = match.format(out, u, std::regex_constants::format_default);

            last_match = match;
        }
        if(last_match.ready())
            std::copy(last_match.suffix().first, last_match.suffix().second, out);

        ss << unescaped_result;
    }

}

/**
 * Writes IRI value to given stringstream, escaping characters as necessary
 * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
 */
void NQuadsSerialization::outputIRI(const std::string & value, std::stringstream & ss) {
    ss << "<";
    ss << value;
    ss << ">";
}

/**
 * Returns escaped input string
 * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
 */
std::string NQuadsSerialization::escape(const std::string &input) {
    std::stringstream ss;
    ::escape(input, ss);
    return ss.str();
}

/**
 * Returns Un-escaped input string
 * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
 */
std::string NQuadsSerialization::unescape(const std::string &input) {
    std::stringstream ss;
    ::unescape(input, ss);
    return ss.str();
}

std::string NQuadsSerialization::toNQuads(const RDF::RDFDataset &dataset) {

    std::vector<std::string> nquads;
    std::stringstream ss;

    for (const auto& quad : dataset.getAllGraphsAsQuads()) {
        nquads.push_back(toNQuad(quad));
    }

    std::sort(nquads.begin(), nquads.end());

    for (const auto& nq : nquads) {
        ss << nq;
    }

    return ss.str();
}

std::string NQuadsSerialization::toNQuad(const RDF::RDFQuad& quad) {
    std::stringstream ss;

    // subject: IRI or bnode (https://www.w3.org/TR/rdf11-concepts/#section-triples)
    std::shared_ptr<RDF::Node> s = quad.getSubject();
    if (s->isIRI())
        outputIRI(s->getValue(), ss);
    else
        ss << s->getValue();

    ss << " ";

    // predicate: only IRI (https://www.w3.org/TR/rdf11-concepts/#section-triples)
    std::shared_ptr<RDF::Node> p = quad.getPredicate();
    if (p->isIRI())
        outputIRI(p->getValue(), ss);
    else
        assert(false);

    ss << " ";

    // object: IRI, bnode or literal (https://www.w3.org/TR/rdf11-concepts/#section-triples)
    std::shared_ptr<RDF::Node> o = quad.getObject();
    if (o->isIRI())
        outputIRI(o->getValue(), ss);
    else if (o->isBlankNode())
        ss << o->getValue();
    else {
        ss << "\"";
        ::escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@" << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            ::escape(o->getDatatype(), ss);
            ss << ">";
        }
    }

    ss << " ";

    // graph: IRI or bnode (https://www.w3.org/TR/rdf11-concepts/#section-triples)
    std::shared_ptr<RDF::Node> g = quad.getGraph();
    if (g != nullptr && g->getValue() != JsonLdConsts::DEFAULT) {
        if (g->isIRI())
            outputIRI(g->getValue(), ss);
        else
            ss << g->getValue();

        ss << " ";
    }

    ss << ".\n";

    return ss.str();
}

std::string NQuadsSerialization::toNQuad(const RDF::RDFTriple& triple) {
    return toNQuad({triple.getSubject(), triple.getPredicate(), triple.getObject(), nullptr});
}

RDF::RDFDataset NQuadsSerialization::parse(std::string input) {
    RDF::RDFDataset dataset((JsonLdOptions()));

    std::vector<std::string> lines = ::splitLines(input);
    std::regex emptyRgx(EMPTY);
    std::regex quadRgx(QUAD);
    std::smatch match;

    int lineNumber = 0;
    for(const auto& line : lines) {
        lineNumber++;

        // skip empty lines
        if (std::regex_match(line, match, emptyRgx))
            continue;

        // parse quad with regex
        if (!std::regex_match(line, match, quadRgx))
            throw JsonLdError(JsonLdError::SyntaxError,
                              "Error while parsing N-Quads; invalid quad. line:" + std::to_string(lineNumber));

        // extract subject from matches
        std::shared_ptr<RDF::Node> subject;
        if(match[QUAD_SUBJECT_AS_IRI].matched)
            subject = std::make_shared<RDF::IRI>(unescape(match[QUAD_SUBJECT_AS_IRI].str()));
        else
            subject = std::make_shared<RDF::BlankNode>(unescape(match[QUAD_SUBJECT_AS_BNODE].str()));

        // extract predicate from matches
        std::shared_ptr<RDF::Node> predicate = std::make_shared<RDF::IRI>(unescape(match[QUAD_PREDICATE].str()));

        // extract object from matches
        std::shared_ptr<RDF::Node> object;
        if(match[QUAD_OBJECT_AS_IRI].matched)
            object = std::make_shared<RDF::IRI>(unescape(match[QUAD_OBJECT_AS_IRI].str()));
        else if(match[QUAD_OBJECT_AS_BNODE].matched)
            object = std::make_shared<RDF::BlankNode>(unescape(match[QUAD_OBJECT_AS_BNODE].str()));
        else {
            std::string language = unescape(match[QUAD_OBJECT_AS_LITERAL_LANGUAGETAG].str());
            std::string datatype;
            if (match[QUAD_OBJECT_AS_LITERAL_DATATYPE].matched)
                datatype = unescape(match[QUAD_OBJECT_AS_LITERAL_DATATYPE].str());
            else {
                if (match[QUAD_OBJECT_AS_LITERAL_LANGUAGETAG].matched)
                    datatype = JsonLdConsts::RDF_LANGSTRING;
                else
                    datatype = JsonLdConsts::XSD_STRING;
            }
            object = std::make_shared<RDF::Literal>(unescape(match[QUAD_OBJECT_AS_LITERAL].str()), &datatype, &language);
        }

        // extract graph name from matches ('@default' is used for the default graph)
        std::string name = "@default";
        if (match[QUAD_GRAPH_AS_IRI].matched) {
            name = unescape(match[QUAD_GRAPH_AS_IRI].str());
        } else if (match[QUAD_GRAPH_AS_BNODE].matched) {
            name = unescape(match[QUAD_GRAPH_AS_BNODE].str());
        }

        // add RDFTriple to graph in dataset
        RDF::RDFTriple triple(subject, predicate, object);
        // note: RDFTriple will not be added again to graph in dataset if it already exists
        dataset.addTripleToGraph(name, triple);
    }

    return dataset;
}

