#ifndef LIBJSONLD_CPP_RDFNODE_H
#define LIBJSONLD_CPP_RDFNODE_H

#include "jsonld-cpp/jsoninc.h"
#include <memory>
#include <string>

// todo: probably need more comments in here

namespace RDF {

    class Node {
    protected:
        nlohmann::ordered_json map;

    public:
        Node();
        virtual ~Node();

        virtual bool isLiteral() const;
        virtual bool isIRI() const;
        virtual bool isBlankNode() const;

        std::string getDatatype() const;
        std::string getLanguage() const;
        std::string getValue() const;

        void setValue(const std::string & s);

        friend bool operator==(const Node& lhs, const Node& rhs);
        friend bool operator!=(const Node& lhs, const Node& rhs);

        friend bool operator<(const Node &lhs, const Node &rhs);
        friend bool operator>(const Node &lhs, const Node &rhs);
        friend bool operator<=(const Node &lhs, const Node &rhs);
        friend bool operator>=(const Node &lhs, const Node &rhs);
    };

    bool operator==(const Node& lhs, const Node& rhs);
    bool operator!=(const Node& lhs, const Node& rhs);


    class Literal : public Node {
    public:
        explicit Literal(const std::string& value, std::string * datatype = nullptr, std::string * language = nullptr);

        bool isLiteral() const override;
    };

    class IRI : public Node {
    public:
        explicit IRI(const std::string& iri);

        bool isIRI() const override;
    };

    class BlankNode : public Node {
    public:
        explicit BlankNode(const std::string& attribute);

        bool isBlankNode() const override;
    };


    struct NodeLess {
        bool operator()(const Node & lhs, const Node & rhs) const;
    };

    struct NodePtrLess {
        bool operator()(const std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs) const;
    };

    struct NodePtrEquals {
        bool operator()(const std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs) const;
    };

    bool operator<(const Node &lhs, const Node &rhs);

    bool operator>(const Node &lhs, const Node &rhs);

    bool operator<=(const Node &lhs, const Node &rhs);

    bool operator>=(const Node &lhs, const Node &rhs);

}

#endif //LIBJSONLD_CPP_RDFNODE_H
