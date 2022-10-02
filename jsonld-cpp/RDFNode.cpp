#include "jsonld-cpp/RDFNode.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/DoubleFormatter.h"

using nlohmann::json;

namespace RDF {

    bool operator==(const Node &lhs, const Node &rhs) {
        return lhs.map == rhs.map;
    }

    bool operator!=(const Node &lhs, const Node &rhs) {
        return !(lhs.map == rhs.map);
    }

    bool operator<(const Node &lhs, const Node &rhs) {
        NodeLess f;
        return f(lhs, rhs);
    }

    bool operator>(const Node &lhs, const Node &rhs) {
        return rhs < lhs;
    }

    bool operator<=(const Node &lhs, const Node &rhs) {
        return !(rhs < lhs);
    }

    bool operator>=(const Node &lhs, const Node &rhs) {
        return !(lhs < rhs);
    }

    std::string Node::getValue() const {
        return map["value"].get<std::string>();
    }

    void Node::setValue(const std::string &s) {
        map["value"] = s;
    }

    std::string Node::getDatatype() const {
        if(map.count("datatype") > 0)
            return map["datatype"].get<std::string>();
        else
            return "";
    }

    std::string Node::getLanguage() const {
        if(map.count("language") > 0)
            return map["language"].get<std::string>();
        else
            return "";
    }

    Node::Node() = default;
    Node::~Node() = default;

    bool Literal::isLiteral() const {
        return true;
    }

    bool Literal::isIRI() const {
        return false;
    }

    bool Literal::isBlankNode() const {
        return false;
    }

    Literal::Literal(const std::string &value, std::string *datatype, std::string *language) {
        map["type"] = "literal";
        map["value"] = value;
        map["datatype"] = datatype != nullptr ? *datatype : JsonLdConsts::XSD_STRING;
        if (language != nullptr) {
            map["language"] = *language;
        }
    }

    bool IRI::isLiteral() const {
        return false;
    }

    bool IRI::isIRI() const {
        return true;
    }

    bool IRI::isBlankNode() const {
        return false;
    }

    IRI::IRI(const std::string &iri) {
        map["type"] = "IRI";
        map["value"] = iri;
    }

    bool BlankNode::isBlankNode() const {
        return true;
    }

    bool BlankNode::isIRI() const {
        return false;
    }

    bool BlankNode::isLiteral() const {
        return false;
    }

    BlankNode::BlankNode(const std::string &attribute) {
        map["type"] = "blank node";
        map["value"] = attribute;
    }

    bool NodeLess::operator()(const Node &lhs, const Node &rhs) const {
        if(lhs.isIRI()) {
            if(!rhs.isIRI())
                return false;
        }
        else if (lhs.isBlankNode()) {
            if(rhs.isIRI())
                return true;
            else if(rhs.isLiteral())
                return false;
        }
        else if(lhs.isLiteral()) {
            if(rhs.isIRI() || rhs.isBlankNode())
                return true;
            else {
                // lhs and rhs are literals
                if(lhs.getValue() != rhs.getValue()) {
                    return lhs.getValue() < rhs.getValue();
                }
                else {
                    if (!lhs.getLanguage().empty() || !rhs.getLanguage().empty())
                        return lhs.getLanguage() < rhs.getLanguage();
                    else
                        return lhs.getDatatype() < rhs.getDatatype();
                }
            }
        }
        return lhs.getValue() < rhs.getValue();
    }

    bool NodePtrLess::operator()(const std::shared_ptr<Node> &lhs, const std::shared_ptr<Node> &rhs) const {
        if(lhs->isIRI()) {
            if(!rhs->isIRI())
                return false;
        }
        else if (lhs->isBlankNode()) {
            if(rhs->isIRI())
                return true;
            else if(rhs->isLiteral())
                return false;
        }
        else if(lhs->isLiteral()) {
            if(rhs->isIRI() || rhs->isBlankNode())
                return true;
            else {
                // lhs and rhs are literals
                if(lhs->getValue() != rhs->getValue()) {
                    return lhs->getValue() < rhs->getValue();
                }
                else {
                    if (!lhs->getLanguage().empty() || !rhs->getLanguage().empty())
                        return lhs->getLanguage() < rhs->getLanguage();
                    else
                        return lhs->getDatatype() < rhs->getDatatype();
                }
            }
        }
        return lhs->getValue() < rhs->getValue();
    }

    bool NodePtrEquals::operator()(const std::shared_ptr<Node> &lhs, const std::shared_ptr<Node> &rhs) const {
        if(lhs == rhs) return true;
        if(lhs && rhs) return *lhs == *rhs;
        return false;
    }

}
