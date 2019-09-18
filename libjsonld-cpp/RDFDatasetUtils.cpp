#include "RDFDatasetUtils.h"

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

std::string RDFDatasetUtils::toNQuads(const RDF::RDFDataset &dataset) {
    std::stringstream ss;

    std::vector<std::string> quads;

    for (auto graphName : dataset.graphNames()) {
        std::vector<RDF::Quad> triples = dataset.getQuads(graphName);
        std::string *graphNamePtr = &graphName;
        if (graphName == "@default") {
            graphNamePtr = nullptr;
        }
        for (auto triple : triples) {
            quads.push_back(toNQuad(triple, graphNamePtr));
        }
    }
    std::sort(quads.begin(), quads.end());
    for (auto quad : quads) {
        ss << quad;
    }

    return ss.str();
}

std::string RDFDatasetUtils::toNQuad(RDF::Quad quad, std::string *graphName) {
    return toNQuad(quad, graphName, nullptr);
}

std::string RDFDatasetUtils::toNQuad(RDF::Quad triple, std::string *graphName, std::string *bnode) {
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
        // normalization mode
    else if (bnode != nullptr) {
        ss << ((s->getValue() == *bnode)  ? "_:a" : "_:z");
    }
        // normal mode
    else {
        ss << s->getValue();
    }

    if (p->isIRI()) {
        ss << " <";
        escape(p->getValue(), ss);
        ss << "> ";
    }
        // otherwise it must be a bnode (TODO: can we only allow this if the flag is set in options?)
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
        // normalization mode
        if (bnode != nullptr) {
            ss << ((o->getValue() == *bnode)  ? "_:a" : "_:z");
        }
        // normal mode
        else {
            ss << o->getValue();
        }
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
        } else if (bnode != nullptr) {
            ss << " _:g";
        } else {
            ss << " ";
            ss << *graphName;
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
 * @param rval
 *            The {@link StringBuilder} to append to.
 */
void RDFDatasetUtils::escape(std::string str, std::stringstream & ss) {
    for (std::string::size_type i = 0; i < str.size(); i++) {
        char hi = str[i];
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

