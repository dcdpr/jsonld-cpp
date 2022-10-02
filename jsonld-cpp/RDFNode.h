#ifndef LIBJSONLD_CPP_RDFNODE_H
#define LIBJSONLD_CPP_RDFNODE_H

#include "jsonld-cpp/jsoninc.h"
#include <memory>
#include <string>

namespace RDF {

    class Node {
    protected:
        nlohmann::json map;

    public:
        Node();
        virtual ~Node();

        virtual bool isLiteral() const = 0;
        virtual bool isIRI() const = 0;
        virtual bool isBlankNode() const = 0;

        std::string getDatatype() const;
        std::string getLanguage() const;
        std::string getValue() const;
        void setValue(const std::string & s); // todo: only used in one place. make a friend?

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
        bool isIRI() const override;
        bool isBlankNode() const override;
    };

    class IRI : public Node {
    public:
        explicit IRI(const std::string& iri);

        bool isLiteral() const override;
        bool isIRI() const override;
        bool isBlankNode() const override;
    };

    class BlankNode : public Node {
    public:
        explicit BlankNode(const std::string& attribute);

        bool isLiteral() const override;
        bool isIRI() const override;
        bool isBlankNode() const override;
    };


    // sort order of Nodes should be: Literals < BlankNodes < IRIs
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
