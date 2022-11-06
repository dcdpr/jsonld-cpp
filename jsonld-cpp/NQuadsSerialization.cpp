#include "jsonld-cpp/NQuadsSerialization.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/RDFRegex.h"
#include "jsonld-cpp/JsonLdError.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <regex>


namespace {

    // return vector of strings after splitting input string into lines
    std::vector<std::string> splitLines(std::string &input) {
        std::vector<std::string> lines;
        std::regex rgx(RDFRegex::EOLN);
        std::sregex_token_iterator i(input.begin(), input.end(), rgx, -1);
        std::sregex_token_iterator end;

        for (; i != end; ++i)
            lines.push_back(*i);
        return lines;
    }

    std::string escape(const std::string &input);

    std::string unescape(const std::string &input);

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

    /**
     * Un-escapes input string, writes output to given stringstream
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    void unescape(const std::string &str, std::stringstream &ss) {
        std::string rval = str;
        if (!str.empty()) {
            std::regex charsRgx(RDFRegex::UCHAR_MATCHED);
            auto chars_begin =
                    std::sregex_iterator(str.begin(), str.end(), charsRgx);
            auto chars_end = std::sregex_iterator();
            for (std::sregex_iterator reg_it = chars_begin; reg_it != chars_end; ++reg_it) {
                const std::smatch &match = *reg_it;
                std::string match_str = match.str();

                // print all matches for debugging:
//            for (size_t i = 0; i < match.size(); ++i) {
//                std::ssub_match sub_match = match[i];
//                std::string piece = sub_match.str();
//                std::cout << "  submatch " << i << ": [" << piece << "]\n";
//            }

                std::string u;
                if (!match[1].matched) {
                    std::string hex = match[2].matched ? match[2].str() : match[3].str();
                    long v = std::stol(hex, nullptr, 16);
                    if (v > 0xFFFF) {
                        throw JsonLdError(JsonLdError::SyntaxError,
                                          "UTF-32 chars not yet supported");
                    } else {
                        auto it = std::back_inserter(u);
                        if (v < 0x80)                        // one octet
                            *(it++) = static_cast<char>(v);
                        else if (v < 0x800) {                // two octets
                            *(it++) = static_cast<char>((v >> 6) | 0xc0);
                            *(it++) = static_cast<char>((v & 0x3f) | 0x80);
                        } else {                             // three octets
                            *(it++) = static_cast<char>((v >> 12) | 0xe0);
                            *(it++) = static_cast<char>(((v >> 6) & 0x3f) | 0x80);
                            *(it++) = static_cast<char>((v & 0x3f) | 0x80);
                        }
                    }
                } else {
                    char c = match[1].str()[0];
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
                        case 'v':
                            u = "\v";
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
                            u = "\\";
                            break;
                        default:
                            continue;
                    }
                }
                std::regex pat(escape(match[0].str()));
                rval = std::regex_replace(rval, pat, u);
            }

        }

        ss << rval;
    }

    /**
     * Returns escaped input string
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    std::string escape(const std::string &input) {
        std::stringstream ss;
        escape(input, ss);
        return ss.str();
    }

    /**
     * Returns Un-escaped input string
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    std::string unescape(const std::string &input) {
        std::stringstream ss;
        unescape(input, ss);
        return ss.str();
    }

    /**
     * Writes IRI value to given stringstream, escaping characters as necessary
     * See N-Quads escaping rules: https://www.w3.org/TR/turtle/#sec-escapes
     */
    void outputIRI(const std::string & value, std::stringstream & ss) {
        ss << "<";
        escape(value, ss);
        ss << ">";
    }

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
        escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@" << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            escape(o->getDatatype(), ss);
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
    // converting an RDFTriple to a nquad might not be a real thing, but it is nice to do
    // so RDFTriple.toString() can quickly convert itself to a string for printing/debugging

    return toNQuad({triple.getSubject(), triple.getObject(), triple.getObject(), nullptr});

}

std::string NQuadsSerialization::toNQuadForNormalization(const RDF::RDFQuad& quad, std::string *bnode) {
    assert(bnode);
    std::stringstream ss;

    // subject: IRI or bnode (https://www.w3.org/TR/rdf11-concepts/#section-triples)
    std::shared_ptr<RDF::Node> s = quad.getSubject();
    if (s->isIRI())
        outputIRI(s->getValue(), ss);
    else {
        assert(s->isBlankNode());
        ss << ((s->getValue() == *bnode) ? "_:a" : "_:z");
    }

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
        ss << ((o->getValue() == *bnode) ? "_:a" : "_:z");
    else {
        ss << "\"";
        escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@" << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            escape(o->getDatatype(), ss);
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
            ss << ((g->getValue() == *bnode) ? "_:a" : "_:z");

        ss << " ";
    }

    ss << ".\n";

    return ss.str();
}



RDF::RDFDataset NQuadsSerialization::parse(std::string input) {
    RDF::RDFDataset dataset((JsonLdOptions()));

    std::vector<std::string> lines = ::splitLines(input);
    static std::regex emptyRgx(RDFRegex::EMPTY);
    static std::regex quadRgx(RDFRegex::QUAD);
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

        // print all matches for debugging:
//        for (size_t i = 0; i < match.size(); ++i) {
//            std::ssub_match sub_match = match[i];
//            std::string piece = sub_match.str();
//            std::cout << "  submatch " << i << ": [" << piece << "]\n";
//        }

        // extract subject from matches
        std::shared_ptr<RDF::Node> subject;
        if(match[1].matched)
            subject = std::make_shared<RDF::IRI>(unescape(match[1].str()));
        else
            subject = std::make_shared<RDF::BlankNode>(unescape(match[2].str()));

        // extract predicate from matches
        std::shared_ptr<RDF::Node> predicate = std::make_shared<RDF::IRI>(unescape(match[3].str()));

        // extract object from matches
        std::shared_ptr<RDF::Node> object;
        if(match[4].matched)
            object = std::make_shared<RDF::IRI>(unescape(match[4].str()));
        else if(match[5].matched)
            object = std::make_shared<RDF::BlankNode>(unescape(match[5].str()));
        else {
            std::string language = unescape(match[8].str());
            std::string datatype;
            if (match[7].matched)
                datatype = unescape(match[7].str());
            else {
                if (match[8].matched)
                    datatype = JsonLdConsts::RDF_LANGSTRING;
                else
                    datatype = JsonLdConsts::XSD_STRING;
            }
            object = std::make_shared<RDF::Literal>(unescape(match[6].str()), &datatype, &language);
        }

        // extract graph name from matches ('@default' is used for the default graph)
        std::string name = "@default";
        if (match[9].matched) {
            name = unescape(match[9].str());
        } else if (match[10].matched) {
            name = unescape(match[10].str());
        }

        // add RDFTriple to graph in dataset
        RDF::RDFTriple triple(subject, predicate, object);
        dataset.addTripleToGraph(name, triple);
    }

    return dataset;
}

