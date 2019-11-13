#ifndef LIBJSONLD_CPP_RDFDATASET_H
#define LIBJSONLD_CPP_RDFDATASET_H

#include "jsoninc.h"
#include "JsonLdConsts.h"
#include "JsonLdOptions.h"
#include "UniqueNamer.h"
#include <memory>
#include <string>
#include <iostream>
#include <set>
#include <utility>

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



    class Quad {
    private:

        std::map<std::string, std::shared_ptr<Node>> map;

        void setSubject(std::shared_ptr<Node> subject) { map["subject"] = std::move(subject); }
        void setPredicate(std::shared_ptr<Node> predicate)  { map["predicate"] = std::move(predicate); }
        void setObject(std::shared_ptr<Node> object)  { map["object"] = std::move(object); }

        void init( std::string subject,  std::string predicate,  std::shared_ptr<Node> object,
                   std::string * graph);

    public:

        Quad(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object,
             std::string * graph);

        Quad( std::string subject,  std::string predicate,  std::string object, std::string * graph);

        Quad( std::string subject,  std::string predicate,  const std::string& value,
              std::string datatype,  std::string language,  std::string * graph);

        std::shared_ptr<Node> getPredicate() const;
        std::shared_ptr<Node> getObject() const;
        std::shared_ptr<Node> getSubject() const;

        std::shared_ptr<Node> getGraph() const;
        void setGraph(const std::string *graph);

        friend bool operator==(const Quad &lhs, const Quad &rhs);
        friend bool operator!=(const Quad &lhs, const Quad &rhs);

        friend bool operator<(const Quad &lhs, const Quad &rhs);
        friend bool operator>(const Quad &lhs, const Quad &rhs);
        friend bool operator<=(const Quad &lhs, const Quad &rhs);
        friend bool operator>=(const Quad &lhs, const Quad &rhs);

    };

    bool operator==(const RDF::Quad &lhs, const RDF::Quad &rhs);
    bool operator!=(const RDF::Quad &lhs, const RDF::Quad &rhs);
    bool operator<(const Quad &lhs, const Quad &rhs);
    bool operator>(const Quad &lhs, const Quad &rhs);
    bool operator<=(const Quad &lhs, const Quad &rhs);
    bool operator>=(const Quad &lhs, const Quad &rhs);

    struct QuadPtrLess :
            public std::binary_function<const std::shared_ptr<Quad>, const std::shared_ptr<Quad>, bool> {
        bool operator()(const std::shared_ptr<Quad> & lhs, const std::shared_ptr<Quad> & rhs) const {
            return *lhs < *rhs ;
        }
    };

    class RDFDataset {
    public:
        typedef std::map<std::string, std::vector<Quad>> VectorMap;
        typedef std::map<std::string, std::string> StringMap;

    private:
        VectorMap vectorMap;

        std::shared_ptr<Node> objectToRDF(nlohmann::json item);

    public:
        JsonLdOptions options;
        UniqueNamer *blankNodeUniqueNamer;

        RDFDataset(JsonLdOptions options, UniqueNamer *blankNodeUniqueNamer);

        VectorMap::mapped_type & at(const VectorMap::key_type& s);
        std::pair<VectorMap::iterator,bool> insert( const VectorMap::value_type& value );

        void graphToRDF(std::string &graphName, const nlohmann::json & graph);

        std::set<std::string> graphNames() const;
        std::vector<Quad> getQuads(const std::string & graphName) const;
    };

}

#endif //LIBJSONLD_CPP_RDFDATASET_H
