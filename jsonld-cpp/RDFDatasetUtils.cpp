#include "jsonld-cpp/RDFDatasetUtils.h"
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

std::string RDFDatasetUtils::toNQuads(const RDF::RDFDataset &dataset) {

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

std::string RDFDatasetUtils::toNQuad(const RDF::RDFQuad& quad) {
    std::stringstream ss;

    std::shared_ptr<RDF::Node> s = quad.getSubject();
    std::shared_ptr<RDF::Node> p = quad.getPredicate();
    std::shared_ptr<RDF::Node> o = quad.getObject();

    // subject is an IRI or bnode
    if (s->isIRI()) {
        ss << "<";
        escape(s->getValue(), ss);
        ss << ">";
    }
    else {
        ss << s->getValue();
    }

    // predicate is either an IRI or bnode
    if (p->isIRI()) {
        ss << " <";
        escape(p->getValue(), ss);
        ss << "> ";
    }
    else {
        ss << " ";
        escape(p->getValue(), ss);
        ss << " ";
    }

    // object is IRI, bnode or literal
    if (o->isIRI()) {
        ss << "<";
        escape(o->getValue(), ss);
        ss << ">";
    } else if (o->isBlankNode()) {
        ss << o->getValue();
    } else {
        ss << "\"";
        escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@";
            ss << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            escape(o->getDatatype(), ss);
            ss << ">";
        }
    }

    // graph
    std::shared_ptr<RDF::Node> g = quad.getGraph();
    if (g != nullptr && g->getValue() != JsonLdConsts::DEFAULT) {
        std::string gn = g->getValue();
        if (!BlankNodeNames::isBlankNodeName(gn)) {
            ss << " <";
            escape(gn, ss);
            ss << ">";
        } else {
            ss << " ";
            ss << gn;
        }
    }

    ss << " .\n";

    return ss.str();
}

std::string RDFDatasetUtils::toNQuad(const RDF::RDFTriple& triple) {
    // converting an RDFTriple to a nquad might not be a real thing, but it is nice to do
    // so RDFTriple.toString can quickly convert itself to a string for printing/debugging

    std::stringstream ss;

    std::shared_ptr<RDF::Node> s = triple.getSubject();
    std::shared_ptr<RDF::Node> p = triple.getPredicate();
    std::shared_ptr<RDF::Node> o = triple.getObject();

    // subject is an IRI or bnode
    if (s->isIRI()) {
        ss << "<";
        escape(s->getValue(), ss);
        ss << ">";
    }
    else {
        ss << s->getValue();
    }

    // predicate is either an IRI or bnode
    if (p->isIRI()) {
        ss << " <";
        escape(p->getValue(), ss);
        ss << "> ";
    }
    else {
        ss << " ";
        escape(p->getValue(), ss);
        ss << " ";
    }

    // object is IRI, bnode or literal
    if (o->isIRI()) {
        ss << "<";
        escape(o->getValue(), ss);
        ss << ">";
    } else if (o->isBlankNode()) {
        ss << o->getValue();
    } else {
        ss << "\"";
        escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@";
            ss << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            escape(o->getDatatype(), ss);
            ss << ">";
        }
    }

    ss << " .\n";

    return ss.str();
}

std::string RDFDatasetUtils::toNQuadForNormalization(const RDF::RDFQuad& quad, std::string *bnode) {
    assert(bnode);
    std::stringstream ss;

    std::shared_ptr<RDF::Node> s = quad.getSubject();
    std::shared_ptr<RDF::Node> p = quad.getPredicate();
    std::shared_ptr<RDF::Node> o = quad.getObject();

    // subject is either an IRI or bnode
    if (s->isIRI()) {
        ss << "<";
        escape(s->getValue(), ss);
        ss << ">";
    }
    else {
        assert(s->isBlankNode());
        ss << ((s->getValue() == *bnode)  ? "_:a" : "_:z");
    }

    // predicate is either an IRI or bnode
    if (p->isIRI()) {
        ss << " <";
        escape(p->getValue(), ss);
        ss << "> ";
    }
    else {
        assert(p->isBlankNode());
        ss << ((p->getValue() == *bnode)  ? "_:a" : "_:z");
    }

    // object is IRI, bnode or literal
    if (o->isIRI()) {
        ss << "<";
        escape(o->getValue(), ss);
        ss << ">";
    } else if (o->isBlankNode()) {
        ss << ((o->getValue() == *bnode)  ? "_:a" : "_:z");
    } else {
        ss << "\"";
        escape(o->getValue(), ss);
        ss << "\"";
        if (o->getDatatype() == JsonLdConsts::RDF_LANGSTRING) {
            ss << "@";
            ss << o->getLanguage();
        } else if (o->getDatatype() != JsonLdConsts::XSD_STRING) {
            ss << "^^<";
            escape(o->getDatatype(), ss);
            ss << ">";
        }
    }

    // graph
    std::shared_ptr<RDF::Node> g = quad.getGraph();
    if (g != nullptr && g->getValue() != JsonLdConsts::DEFAULT) {
        std::string gn = g->getValue();
        if (!BlankNodeNames::isBlankNodeName(gn)) {
            ss << " <";
            escape(gn, ss);
            ss << ">";
        } else {
            ss << ((gn == *bnode)  ? " _:a" : " _:z");
        }
    }

    ss << " .\n";

    return ss.str();
}

/**
 * Escapes the given string according to the N-Quads escape rules
 *
 * @param str
 *            The string to escape
 * @param ss
 *            The stringstream to append to.
 */
void RDFDatasetUtils::escape(const std::string& str, std::stringstream & ss) {
    for (char hi : str) {
        if (hi <= 0x8 || hi == 0xB || hi == 0xC ||
            (hi >= 0xE && hi <= 0x1F) ||
            (hi >= 0x7F && hi <= 0xA0)) {
            ss << "\\u";
            ss << std::setfill('0') << std::setw(4) << hi; //%04x", (int) hi));
        }
        else {
            switch (hi) {
                case '\b':
                    ss << "\\b";
                    break;
                case '\n':
                    ss << "\\n";
                    break;
                case '\t':
                    ss << "\\t";
                    break;
//                case '\f':
//                    ss << "\\f";
//                    break;
                case '\r':
                    ss << "\\r";
                    break;
                case '\"':
                    ss << "\\\"";
                    break;
                case '\\':
                    ss << "\\\\";
                    break;
                default:
                    // just put the char as is
                    ss << hi;
                    break;
            }
        }
    }
}

void RDFDatasetUtils::unescape(const std::string& str, std::stringstream & ss) {

    std::string rval = str;

    if(!str.empty()) {

        std::regex charsRgx(RDFRegex::UCHAR_MATCHED);
        auto chars_begin =
                std::sregex_iterator(str.begin(), str.end(), charsRgx);
        auto chars_end = std::sregex_iterator();
        for (std::sregex_iterator reg_it = chars_begin; reg_it != chars_end; ++reg_it) {
            std::smatch match = *reg_it;
            std::string match_str = match.str();

            // print all matches for debugging:
            for (size_t i = 0; i < match.size(); ++i) {
                std::ssub_match sub_match = match[i];
                std::string piece = sub_match.str();
            }

            std::string uni;
            if(!match[1].matched) {
                std::string hex = match[2].matched ? match[2].str() : match[3].str();
                long v = std::stol(hex, nullptr, 16);
                if (v > 0xFFFF) {
                    throw JsonLdError(JsonLdError::SyntaxError,
                                      "UTF-32 chars not yet supported");
                }
                else {
                    auto it = std::back_inserter(uni);
                    if (v < 0x80)                        // one octet
                        *(it++) = static_cast<char>(v);
                    else if (v < 0x800) {                // two octets
                        *(it++) = static_cast<char>((v >> 6)            | 0xc0);
                        *(it++) = static_cast<char>((v & 0x3f)          | 0x80);
                    }
                    else if (v < 0x10000) {              // three octets
                        *(it++) = static_cast<char>((v >> 12)           | 0xe0);
                        *(it++) = static_cast<char>(((v >> 6) & 0x3f)   | 0x80);
                        *(it++) = static_cast<char>((v & 0x3f)          | 0x80);
                    }
                }
            }
            else {
                char c = match[1].str()[0];
                switch (c) {
                    case 'b':
                        uni = "\b";
                        break;
                    case 'n':
                        uni = "\n";
                        break;
                    case 't':
                        uni = "\t";
                        break;
                    case 'f':
                        uni = "\f";
                        break;
                    case 'r':
                        uni = "\r";
                        break;
                    case '\'':
                        uni = "'";
                        break;
                    case '\"':
                        uni = "\"";
                        break;
                    case '\\':
                        uni = "\\";
                        break;
                    default:
                        continue;
                }
            }
            std::regex pat(escape(match[0].str()));
            rval = std::regex_replace(rval, pat, uni);
        }

    }

    ss << rval;
}

std::string RDFDatasetUtils::unescape(const std::string& str) {
    std::stringstream ss;
    unescape(str, ss);
    return ss.str();
}

std::string RDFDatasetUtils::escape(const std::string& str) {
    std::stringstream ss;
    escape(str, ss);
    return ss.str();
}

/**
 * Parses a string containing RDF data in the form of N-Quads.
 *
 * @param input
 *            the N-Quads input string to parse.
 *
 * @return an RDF dataset.
 */
RDF::RDFDataset RDFDatasetUtils::parseNQuads(std::string input) {
    RDF::RDFDataset dataset((JsonLdOptions()));

    std::vector<std::string> lines = splitLines(input);

    int lineNumber = 0;
    for(const auto& line : lines) {
        lineNumber++;

        // skip empty lines
        std::regex emptyRgx(RDFRegex::EMPTY);
        std::smatch match;

        if (std::regex_match(line, match, emptyRgx))
            continue;

        // parse quad
        std::regex quadRgx(RDFRegex::QUAD);
        if (!std::regex_match(line, match, quadRgx))
            throw JsonLdError(JsonLdError::SyntaxError,
                                  "Error while parsing N-Quads; invalid quad. line:" + std::to_string(lineNumber));

        // print all matches for debugging:
        for (size_t i = 0; i < match.size(); ++i) {
            std::ssub_match sub_match = match[i];
            std::string piece = sub_match.str();
        }
        // get subject
        std::shared_ptr<RDF::Node> subject;
        if(match[1].matched)
            subject = std::make_shared<RDF::IRI>(unescape(match[1].str()));
        else
            subject = std::make_shared<RDF::BlankNode>(unescape(match[2].str()));

        // get predicate
        std::shared_ptr<RDF::Node> predicate = std::make_shared<RDF::IRI>(unescape(match[3].str()));

        // get object
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

        // get graph name ('@default' is used for the default graph)
        std::string name = "@default";
        if (match[9].matched) {
            name = unescape(match[9].str());
        } else if (match[10].matched) {
            name = unescape(match[10].str());
        }

        RDF::RDFTriple triple(subject, predicate, object);
        dataset.addTripleToGraph(name, triple);
    }

    return dataset;
}

std::vector<std::string> RDFDatasetUtils::splitLines(std::string &input) {
    std::vector<std::string> lines;
    std::regex rgx(RDFRegex::EOLN);
    std::sregex_token_iterator i(input.begin(), input.end(), rgx,-1);
    std::sregex_token_iterator end;

    for ( ; i != end; ++i)
        lines.push_back(*i);
    return lines;
}

