#ifndef LIBJSONLD_CPP_RDFDATASET_H
#define LIBJSONLD_CPP_RDFDATASET_H

#include <memory>
#include <string>
#include <iostream>
#include <set>
#include "json.hpp"
#include "JsonLdConsts.h"
#include "JsonLdOptions.h"
#include "UniqueIdentifierGenerator.h"

namespace RDF {

    class Node {
    protected:
    public:
        friend bool operator<(const Node &lhs, const Node &rhs);

        friend bool operator>(const Node &lhs, const Node &rhs);

        friend bool operator<=(const Node &lhs, const Node &rhs);

        friend bool operator>=(const Node &lhs, const Node &rhs);

    protected:
        nlohmann::json map;

    public:

        virtual bool isLiteral() const = 0;
        virtual bool isIRI() const = 0;
        virtual bool isBlankNode() const = 0;

        std::string getValue() const {
            return map["value"];
        }

        void setValue(const std::string & s) {
            map["value"] = s;
        }

        std::string getDatatype() const {
            if(map.count("datatype") > 0)
                return map["datatype"];
            else
                return "";
        }

        std::string getLanguage() const {
            if(map.count("language") > 0)
                return map["language"];
            else
                return "";
        }

        friend bool operator==(const Node& lhs, const Node& rhs);
        friend bool operator!=(const Node& lhs, const Node& rhs);


    };

    bool operator==(const Node& lhs, const Node& rhs);

    bool operator!=(const Node& lhs, const Node& rhs);


    class Literal : public Node {

    public:
        explicit Literal(std::string value, std::string * datatype = nullptr, std::string * language = nullptr) {
            map["type"] = "literal";
            map["value"] = value;
            map["datatype"] = datatype != nullptr ? *datatype : JsonLdConsts::XSD_STRING;
            if (language != nullptr) {
                map["language"] = *language;
            }
        }

        bool isLiteral() const override {
            return true;
        }

        bool isIRI() const override {
            return false;
        }

        bool isBlankNode() const override {
            return false;
        }

};

    class IRI : public Node {

    public:
        explicit IRI(std::string iri) {
            map["type"] = "IRI";
            map["value"] = iri;
        }

        bool isLiteral() const override {
            return false;
        }

        bool isIRI() const override {
            return true;
        }

        bool isBlankNode() const override {
            return false;
        }
    };

    class BlankNode : public Node {

    public:
        explicit BlankNode(std::string attribute) {
            map["type"] = "blank node";
            map["value"] = attribute;
        }

        bool isLiteral() const override {
            return false;
        }

        bool isIRI() const override {
            return false;
        }

        bool isBlankNode() const override {
            return true;
        }
    };


    // sort order of Nodes should be: Literals < BlankNodes < IRIs
    struct NodeLess
    {
        bool operator()(const Node & lhs, const Node & rhs) const {
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
    };

    struct NodePtrLess
    {
        bool operator()(const std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs) const {
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
    };

    struct NodePtrEquals {
        bool operator()(const std::shared_ptr<Node> & lhs, const std::shared_ptr<Node> & rhs) const {
            if(lhs == rhs) return true;
            if(lhs && rhs) return *lhs == *rhs;
            return false;
        }
    };

    bool operator<(const Node &lhs, const Node &rhs);

    bool operator>(const Node &lhs, const Node &rhs);

    bool operator<=(const Node &lhs, const Node &rhs);

    bool operator>=(const Node &lhs, const Node &rhs);



    class Quad {
    private:
    public:
        friend bool operator==(const Quad &lhs, const Quad &rhs);
        friend bool operator!=(const Quad &lhs, const Quad &rhs);

        friend bool operator<(const Quad &lhs, const Quad &rhs);
        friend bool operator>(const Quad &lhs, const Quad &rhs);
        friend bool operator<=(const Quad &lhs, const Quad &rhs);
        friend bool operator>=(const Quad &lhs, const Quad &rhs);

        void setGraph(const std::string *graph)  { // todo: maybe make this private? friends needed?
            if (graph != nullptr && *graph != "@default") {
                std::shared_ptr<Node> n;
                graph->find_first_of("_:") == 0 ?
                        n = std::make_shared<BlankNode>(*graph) :
                        n = std::make_shared<IRI>(*graph);
                map["name"] = n;
                printMap();
            }
        }

    private:
        std::map<std::string, std::shared_ptr<Node>> map;

        void printMap() {
            std::cout << "map: size=" << map.size() << std::endl;
            for(const auto& e : map) {
                std::cout << " " << e.first << ": " << e.second << std::endl;
            }
        }
        void setSubject(std::shared_ptr<Node> subject) { map["subject"] = subject; printMap(); }
        void setPredicate(std::shared_ptr<Node> predicate)  { map["predicate"] = predicate; printMap(); }
        void setObject(std::shared_ptr<Node> object)  { map["object"] = object; printMap(); }

    public:

        Quad(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object,
             std::string * graph) {
            setSubject(subject);
            setPredicate(predicate);
            setObject(object);
            setGraph(graph);
        }

        Quad( std::string subject,  std::string predicate,  std::string object, std::string * graph) {
            std::shared_ptr<Node> o;
            object.find_first_of("_:") == 0 ?
                    o = std::make_shared<BlankNode>(object) :
                    o = std::make_shared<IRI>(object);

            init(subject, predicate, o, graph);
        }

        Quad( std::string subject,  std::string predicate,  std::string value,
              std::string datatype,  std::string language,  std::string * graph) {
            init(subject, predicate, std::make_shared<Literal>(value, &datatype, &language), graph);
        }

        std::shared_ptr<Node> getSubject() const {
            if(map.count("subject"))
                return map.at("subject");
            else
                return nullptr;
        }

        std::shared_ptr<Node> getPredicate() const {
            if(map.count("predicate"))
                return map.at("predicate");
            else
                return nullptr;
        }

        std::shared_ptr<Node> getObject() const {
            if(map.count("object"))
                return map.at("object");
            else
                return nullptr;
        }

        std::shared_ptr<Node> getGraph() const {
            if(map.count("name"))
                return map.at("name");
            else
                return nullptr;
        }

//        void setSubject(std::string s) {
//            s.find_first_of("_:") == 0 ?
//            setSubject(std::make_shared<BlankNode>(s)) :
//            setSubject(std::make_shared<IRI>(s));
//        }
        //void setPredicate(std::shared_ptr<Node> predicate)  { map["predicate"] = predicate; printMap(); }
        //void setObject(std::shared_ptr<Node> object)  { map["object"] = object; printMap(); }


    private:

        void init( std::string subject,  std::string predicate,  std::shared_ptr<Node> object,
                   std::string * graph) {
            subject.find_first_of("_:") == 0 ?
            setSubject(std::make_shared<BlankNode>(subject)) :
            setSubject(std::make_shared<IRI>(subject));

            setPredicate(std::make_shared<IRI>(predicate));

            setObject(object);
            setGraph(graph);
        }

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
        StringMap contextMap; // todo: why not use a Context object?
        VectorMap vectorMap;

        std::shared_ptr<Node> objectToRDF(nlohmann::json item);

    public:
        //JsonLdApi api;
        JsonLdOptions options;
        UniqueIdentifierGenerator *blankNodeIdGenerator;

        RDFDataset();
        RDFDataset(JsonLdOptions options, UniqueIdentifierGenerator *blankNodeIdGenerator);

        VectorMap::mapped_type & at(const VectorMap::key_type& s);
        size_t erase( const std::string& key );
        std::pair<VectorMap::iterator,bool> insert( const VectorMap::value_type& value );
        size_t count( const std::string& key ) const;

        void graphToRDF(std::string &graphName, const nlohmann::json & graph);

        std::set<std::string> graphNames() const;
        std::vector<Quad> getQuads(const std::string & graphName) const;
    };

}


#endif //LIBJSONLD_CPP_RDFDATASET_H
