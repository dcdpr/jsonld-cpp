#include "RDFDataset.h"
#include "JsonLdOptions.h"
#include "JsonLdUtils.h"
#include "DoubleFormatter.h"

using VectorMap = RDF::RDFDataset::VectorMap;
using nlohmann::json;

namespace RDF {

    RDF::RDFDataset::RDFDataset() {
        insert(std::make_pair("@default", std::vector<Quad>()));
    }

    RDF::RDFDataset::RDFDataset(JsonLdOptions options, UniqueIdentifierGenerator *blankNodeIdGenerator)
            : options(std::move(options)), blankNodeIdGenerator(blankNodeIdGenerator) {

    }


    VectorMap::mapped_type &RDF::RDFDataset::at(const VectorMap::key_type &s) {
        return vectorMap.at(s);
    }

    size_t RDF::RDFDataset::erase(const std::string &key) {
        return 0;
    }

    size_t RDF::RDFDataset::count(const std::string &key) const {
        return 0;
    }

    std::pair<VectorMap::iterator, bool>
    RDF::RDFDataset::insert(const std::map<std::string, std::vector<Quad>>::value_type &value) {
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
        // what java does with the list of quads later. maybe they sort when printing or comparing?

        // 4.3)
        std::vector<std::string> subjects;
        if(graph.contains("key_insertion_order")) {
            for (json::const_iterator it = graph["key_insertion_order"].begin();
                 it != graph["key_insertion_order"].end(); ++it) {
                subjects.push_back(*it);
            }
        }
        // else?

        for (auto id : subjects) {
            if (JsonLdUtils::isRelativeIri(id)) {
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
                else if (property.find_first_of("_:") == 0 && !options.getProduceGeneralizedRdf()) {
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
                            firstBNode = std::make_shared<BlankNode>(blankNodeIdGenerator->generate());
                        }
                        triples.push_back(Quad(subject, predicate, firstBNode, &graphName));
                        if (!list.empty()) {
                            for (json::size_type i = 0; i < list.size() - 1; i++) {
                                std::shared_ptr<Node> object = objectToRDF(list.at(i));
                                triples.push_back(Quad(firstBNode, rdf_first, object, &graphName));
                                std::shared_ptr<Node> restBNode = std::make_shared<BlankNode>(blankNodeIdGenerator->generate());
                                triples.push_back(Quad(firstBNode, rdf_rest, restBNode, &graphName));
                                firstBNode = restBNode;
                            }
                        }
                        if (last != nullptr) {
                            triples.push_back(Quad(firstBNode, rdf_first, last, &graphName));
                            triples.push_back(Quad(firstBNode, rdf_rest, rdf_nil, &graphName));
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
                if (JsonLdUtils::isRelativeIri(id)) {
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

    std::set<std::string> RDFDataset::graphNames() const {
        std::set<std::string> names;
        for (auto it = vectorMap.begin(); it != vectorMap.end(); ++it) {
            names.insert(it->first);
        }
        return names;
    }

    std::vector<Quad> RDFDataset::getQuads(const std::string & graphName) const {
        if(vectorMap.count(graphName))
            return vectorMap.at(graphName);
        return std::vector<Quad>();
    }

}
