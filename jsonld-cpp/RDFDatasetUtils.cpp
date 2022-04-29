#include "jsonld-cpp/RDFDatasetUtils.h"
#include "jsonld-cpp/RDFRegex.h"
#include "jsonld-cpp/JsonLdError.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <regex>

std::string RDFDatasetUtils::toNQuads(const RDF::RDFDataset &dataset) {
    std::stringstream ss;

    std::vector<std::string> quads;

    for (auto graphName : dataset.graphNames()) {
        std::vector<RDF::Quad> triples = dataset.getQuads(graphName);
        std::string *graphNamePtr = &graphName;
        if (graphName == "@default") {
            graphNamePtr = nullptr;
        }
        for (const auto& triple : triples) {
            quads.push_back(toNQuad(triple, graphNamePtr));
        }
    }
    std::sort(quads.begin(), quads.end());
    for (const auto& quad : quads) {
        ss << quad;
    }

    return ss.str();
}

std::string RDFDatasetUtils::toNQuad(const RDF::Quad& triple, std::string *graphName) {
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

    // graph
    if (graphName != nullptr) {
        if (graphName->find_first_of("_:") != 0) {
            ss << " <";
            escape(*graphName, ss);
            ss << ">";
        } else {
            ss << " ";
            ss << *graphName;
        }
    }

    ss << " .\n";

    return ss.str();
}

std::string RDFDatasetUtils::toNQuadForNormalization(const RDF::Quad& triple, std::string *graphName, std::string *bnode) {
    assert(bnode);
    std::stringstream ss;

    std::shared_ptr<RDF::Node> s = triple.getSubject();
    std::shared_ptr<RDF::Node> p = triple.getPredicate();
    std::shared_ptr<RDF::Node> o = triple.getObject();

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
    if (graphName != nullptr) {
        if (graphName->find_first_of("_:") != 0) {
            ss << " <";
            escape(*graphName, ss);
            ss << ">";
        } else {
            ss << ((*graphName == *bnode)  ? " _:a" : " _:z");
        }
    }

    ss << " .\n";

    return ss.str();
}

bool RDFDatasetUtils::isHighSurrogate(char c) {
    return false;
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
            (hi >= 0x7F && hi <= 0xA0) // 0xA0 is end of non-printable latin-1 supplement characters
//            ||
//            (hi >= 0x24F // 0x24F is the end of latin extensions
//            && !isHighSurrogate(hi))
                ) {
            ss << "\\u";
            ss << std::setfill('0') << std::setw(4) << hi; //%04x", (int) hi));
        }
//        else if (Character.isHighSurrogate(hi)) {
//            final char lo = str.charAt(++i);
//            final int c = (hi << 10) + lo + (0x10000 - (0xD800 << 10) - 0xDC00);
//            rval.append(String.format("\\U%08x", c));
//        }
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
        for (std::sregex_iterator i = chars_begin; i != chars_end; ++i) {
            std::smatch match = *i;
            std::string match_str = match.str();

            // print all matches for debugging:
            for (size_t i = 0; i < match.size(); ++i) {
                std::ssub_match sub_match = match[i];
                std::string piece = sub_match.str();
                std::cout << "  submatch " << i << ": [" << piece << "]\n";
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
    UniqueNamer blankNodeUniqueNamer;
    RDF::RDFDataset dataset(JsonLdOptions(), &blankNodeUniqueNamer);

    // split N-Quad input into lines
    std::vector<std::string> lines;
    std::regex rgx(RDFRegex::EOLN);
    std::sregex_token_iterator i(input.begin(), input.end(), rgx,-1);
    std::sregex_token_iterator end;

    for ( ; i != end; ++i)
        lines.push_back(*i);

    int lineNumber = 0;
    for(auto line : lines) {
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
            std::cout << "  submatch " << i << ": [" << piece << "]\n";
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
        RDF::RDFGraphName gname = RDF::RDFGraphName::createRDFGraphName(name, RDF::RDFGraphName::DEFAULT);
        if (match[9].matched) {
            name = unescape(match[9].str());
            gname = RDF::RDFGraphName::createRDFGraphName(name, RDF::RDFGraphName::IRI);
        } else if (match[10].matched) {
            name = unescape(match[10].str());
            gname = RDF::RDFGraphName::createRDFGraphName(name, RDF::RDFGraphName::BLANKNODE);
        }

        RDF::Quad triple(subject, predicate, object, &name);

        dataset.addQuad(name, triple);
        dataset.addQuad(gname, triple);

    }

    return dataset;
}

