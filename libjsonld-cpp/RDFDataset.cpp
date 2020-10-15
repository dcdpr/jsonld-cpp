#include "RDFDataset.h"
#include "JsonLdOptions.h"
#include "JsonLdUtils.h"
#include "DoubleFormatter.h"
#include "BlankNode.h"

using VectorMap = RDF::RDFDataset::VectorMap;
using nlohmann::json;

namespace RDF {

    RDF::RDFDataset::RDFDataset(JsonLdOptions ioptions, UniqueNamer *iblankNodeUniqueNamer)
            : options(std::move(ioptions)), blankNodeUniqueNamer(iblankNodeUniqueNamer) {

    }

    VectorMap::mapped_type &RDF::RDFDataset::at(const VectorMap::key_type &s) {
        return vectorMap.at(s);
    }

    std::pair<VectorMap::iterator, bool>
    RDF::RDFDataset::insert(const VectorMap::value_type &value) {
        return vectorMap.insert(value);
    }

/**
 * Creates an array of RDF triples for the given graph.
 *
 * @param graphName
 *            The graph URI
 * @param graph
 *            the graph to create RDF triples for.
 */
    void RDF::RDFDataset::graphToRDF(std::string &graphName, const json & graph) {

        std::shared_ptr<Node> rdf_first = std::make_shared<IRI>(JsonLdConsts::RDF_FIRST);
        std::shared_ptr<Node> rdf_rest = std::make_shared<IRI>(JsonLdConsts::RDF_REST);
        std::shared_ptr<Node> rdf_nil = std::make_shared<IRI>(JsonLdConsts::RDF_NIL);


        // 4.2)
        std::vector<Quad> triples;

        //todo: so, wait! ... will rdfdataset ever needs to store shared_ptr to quads, or just quads? If
        // just quads, did I have to go through all that bother with shared_ptr comparisons? probably need to see
        // what java does with the list of quads later.

        // 4.3)
        std::vector<std::string> subjects;
        if(graph.contains("key_insertion_order")) {
            for (const auto & it : graph["key_insertion_order"]) {
                subjects.push_back(it);
            }
        }
        // else?

        for (auto id : subjects) {
            if (!::BlankNode::isBlankNodeName(id) && JsonLdUtils::isRelativeIri(id)) {
                continue;
            }
            const json & node = graph[id];

            std::vector<std::string> properties;
            for (json::const_iterator it = node.begin(); it != node.end(); ++it) {
                properties.push_back(it.key());
            }

            std::sort(properties.begin(), properties.end());

            for (auto property : properties) {
                const json * values;
                // 4.3.2.1)
                if (property == "@type") {
                    values = &node["@type"];
                    property = JsonLdConsts::RDF_TYPE;
                }
                    // 4.3.2.2)
                else if (JsonLdUtils::isKeyword(property)) {
                    continue;
                }
                    // 4.3.2.3)
                else if (::BlankNode::isBlankNodeName(property) && !options.getProduceGeneralizedRdf()) {
                    continue;
                }
                    // 4.3.2.4)
                else if (JsonLdUtils::isRelativeIri(property)) {
                    continue;
                } else {
                    values = &node[property];
                }

                std::shared_ptr<Node> subject;
                if (id.find_first_of("_:") == 0) {
                    // NOTE: don't rename, just set it as a blank node
                    subject = std::make_shared<BlankNode>(id);
                } else {
                    subject = std::make_shared<IRI>(id);
                }

                // RDF predicates
                std::shared_ptr<Node> predicate;
                if (property.find_first_of("_:") == 0) {
                    predicate = std::make_shared<BlankNode>(property);
                } else {
                    predicate = std::make_shared<IRI>(property);
                }

                for (auto item : *values) {
                    // convert @list to triples
                    if (JsonLdUtils::isList(item)) {
                        auto & list = item["@list"];
                        std::shared_ptr<Node> last;
                        std::shared_ptr<Node> firstBNode = rdf_nil;
                        if (!list.empty()) {
                            last = objectToRDF(list.back());
                            firstBNode = std::make_shared<BlankNode>(blankNodeUniqueNamer->get());
                        }
                        triples.emplace_back(subject, predicate, firstBNode, &graphName);
                        if (!list.empty()) {
                            for (json::size_type i = 0; i < list.size() - 1; i++) {
                                std::shared_ptr<Node> object = objectToRDF(list.at(i));
                                triples.emplace_back(firstBNode, rdf_first, object, &graphName);
                                std::shared_ptr<Node> restBNode = std::make_shared<BlankNode>(
                                        blankNodeUniqueNamer->get());
                                triples.emplace_back(firstBNode, rdf_rest, restBNode, &graphName);
                                firstBNode = restBNode;
                            }
                        }
                        if (last != nullptr) {
                            triples.emplace_back(firstBNode, rdf_first, last, &graphName);
                            triples.emplace_back(firstBNode, rdf_rest, rdf_nil, &graphName);
                        }
                    }
                        // convert value or node object to triple
                    else {
                        std::shared_ptr<Node> object = objectToRDF(item);
                        if (object != nullptr) {
                            triples.push_back(Quad(subject, predicate, object, &graphName));
                        }
                    }
                }
            }
        }

        insert(std::make_pair(graphName, triples));

    }

    std::shared_ptr<RDF::Node> RDF::RDFDataset::objectToRDF(json item) {
        // convert value object to RDF
        if (JsonLdUtils::isValue(item)) {
            json value = item["@value"];
            json datatype = item["@type"];
            std::string datatypeStr;
            if (!datatype.is_null())
                datatypeStr = datatype.get<std::string>();

            // convert to XSD datatypes as appropriate
            if (value.is_boolean()) {
                if (datatype.is_null())
                    datatypeStr = JsonLdConsts::XSD_BOOLEAN;
                bool b = value;
                std::stringstream ss;
                ss << std::boolalpha << b;
                return std::make_shared<Literal>(
                        ss.str(),
                        &datatypeStr,
                        nullptr);
            }
            if (value.is_number()) {
                if (value.is_number_float() || datatype == JsonLdConsts::XSD_DOUBLE) {
                    if (datatype.is_null())
                        datatypeStr = JsonLdConsts::XSD_DOUBLE;
                    double d = value;
                    return std::make_shared<Literal>(
                            DoubleFormatter::format(d),
                            &datatypeStr,
                            nullptr);
                } else {
                    if (datatype.is_null())
                        datatypeStr = JsonLdConsts::XSD_INTEGER;
                    int i = value;
                    return std::make_shared<Literal>(
                            std::to_string(i),
                            &datatypeStr,
                            nullptr);
                }
            } else if (item.contains("@language")) {
                std::string languageStr = item["@language"].get<std::string>();
                if (datatype.is_null())
                    datatypeStr = JsonLdConsts::RDF_LANGSTRING;
                return std::make_shared<Literal>(
                        value.get<std::string>(),
                        &datatypeStr,
                        &languageStr);
            } else {
                if (datatype.is_null())
                    datatypeStr = JsonLdConsts::XSD_STRING;
                return std::make_shared<Literal>(
                        value.get<std::string>(),
                        &datatypeStr,
                        nullptr);
            }
        }
            // convert string/node object to RDF
        else {
            std::string id;
            if (JsonLdUtils::isObject(item)) {
                id = item["@id"];
                if (!::BlankNode::isBlankNodeName(id) && JsonLdUtils::isRelativeIri(id)) {
                    return nullptr;
                }
            } else {
                id = item;
            }
            if (id.find_first_of("_:") == 0) {
                // NOTE: once again no need to rename existing blank nodes
                return std::make_shared<BlankNode>(id);
            } else {
                return std::make_shared<IRI>(id);
            }
        }
    }

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
        return map["value"];
    }

    void Node::setValue(const std::string &s) {
        map["value"] = s;
    }

    std::string Node::getDatatype() const {
        if(map.count("datatype") > 0)
            return map["datatype"];
        else
            return "";
    }

    std::string Node::getLanguage() const {
        if(map.count("language") > 0)
            return map["language"];
        else
            return "";
    }

    Node::Node() = default;
    Node::~Node() = default;

    bool operator==(const RDF::Quad &lhs, const RDF::Quad &rhs) {

        // there is no easy way to check that two maps of shared_ptrs are equal, so we will do it
        // the hard way (first check sizes, then compare keys and values with a lambda comparator)

        return
                lhs.map.size() == rhs.map.size() &&
                std::equal(lhs.map.begin(), lhs.map.end(), rhs.map.begin(),
                           [](const decltype(lhs.map)::value_type &a,
                              const decltype(rhs.map)::value_type &b) {
                               return (a.first == b.first) &&
                                      (*(a.second) == *(b.second));
                           });

    }

    bool operator!=(const RDF::Quad &lhs, const RDF::Quad &rhs) {
        return !(rhs == lhs);
    }

    bool operator<(const Quad &lhs, const Quad &rhs) {
        NodePtrLess less;
        NodePtrEquals equals;
        if(!equals(lhs.getGraph(), rhs.getGraph()))
            return less(lhs.getGraph(), rhs.getGraph());
        if(!equals(lhs.getSubject(), rhs.getSubject()))
            return less(lhs.getSubject(), rhs.getSubject());
        if(!equals(lhs.getPredicate(), rhs.getPredicate()))
            return less(lhs.getPredicate(), rhs.getPredicate());

        return less(lhs.getObject(), rhs.getObject());
    }

    bool operator>(const Quad &lhs, const Quad &rhs) {
        return rhs < lhs;
    }

    bool operator<=(const Quad &lhs, const Quad &rhs) {
        return !(rhs < lhs);
    }

    bool operator>=(const Quad &lhs, const Quad &rhs) {
        return !(lhs < rhs);
    }

    void Quad::init(std::string subject, std::string predicate, std::shared_ptr<Node> object, std::string *graph) {
        subject.find_first_of("_:") == 0 ?
        setSubject(std::make_shared<BlankNode>(subject)) :
        setSubject(std::make_shared<IRI>(subject));

        setPredicate(std::make_shared<IRI>(predicate));

        setObject(std::move(object));
        setGraph(graph);
    }

    Quad::Quad(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object,
               std::string *graph) {
        setSubject(std::move(subject));
        setPredicate(std::move(predicate));
        setObject(std::move(object));
        setGraph(graph);
    }

    Quad::Quad(std::string subject, std::string predicate, std::string object, std::string *graph) {
        std::shared_ptr<Node> o;
        object.find_first_of("_:") == 0 ?
                o = std::make_shared<BlankNode>(object) :
                o = std::make_shared<IRI>(object);

        init(std::move(subject), std::move(predicate), o, graph);
    }

    Quad::Quad(std::string subject, std::string predicate, const std::string &value, std::string datatype,
               std::string language, std::string *graph) {
        init(std::move(subject), std::move(predicate), std::make_shared<Literal>(value, &datatype, &language), graph);
    }

    std::shared_ptr<Node> Quad::getPredicate() const {
        if(map.count("predicate"))
            return map.at("predicate");
        else
            return nullptr;
    }

    std::shared_ptr<Node> Quad::getObject() const {
        if(map.count("object"))
            return map.at("object");
        else
            return nullptr;
    }

    std::shared_ptr<Node> Quad::getGraph() const {
        if(map.count("name"))
            return map.at("name");
        else
            return nullptr;
    }

    std::shared_ptr<Node> Quad::getSubject() const {
        if(map.count("subject"))
            return map.at("subject");
        else
            return nullptr;
    }

    void Quad::setGraph(const std::string *graph) { // todo: maybe make this private? friends needed?
        if (graph != nullptr && *graph != "@default") {
            std::shared_ptr<Node> n;
            graph->find_first_of("_:") == 0 ?
                    n = std::make_shared<BlankNode>(*graph) :
                    n = std::make_shared<IRI>(*graph);
            map["name"] = n;
        }
    }

    std::set<std::string> RDFDataset::graphNames() const {
        std::set<std::string> names;
        for (const auto & it : vectorMap) {
            names.insert(it.first);
        }
        return names;
    }

    std::vector<Quad> RDFDataset::getQuads(const std::string & graphName) const {
        if(vectorMap.count(graphName))
            return vectorMap.at(graphName);
        return std::vector<Quad>();
    }

    void RDFDataset::addQuad(std::string name, const Quad& quad) {
        if(!vectorMap.count(name)) {
            // initialise graph in dataset
            std::vector<Quad> v {quad};
            vectorMap.insert(std::make_pair(name, v));
        }
        else {
            // add triple if unique to its graph
            if (std::find(vectorMap[name].begin(), vectorMap[name].end(), quad) == vectorMap[name].end()) {
                vectorMap[name].push_back(quad);
            }
        }
    }

    int RDFDataset::size() const {
        return 0;
    }

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

    bool areIsomorphic(const RDFDataset & dataset1, const RDFDataset & dataset2) {
//        if(dataset1.getQuads())

        return false;
    }

}
