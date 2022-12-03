#include "jsonld-cpp/RDFSerializationProcessor.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/DoubleFormatter.h"

using nlohmann::json;


namespace {

    using namespace RDF;
    using RDF::BlankNode;

    void generateNodeMap(json & element, json &nodeMap, BlankNodeNames &blankNodeNames, std::string *activeGraph, nlohmann::json *activeSubject,
                         std::string *activeProperty, json *list)
    {
        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the node map generation algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#node-map-generation

        // Also including some of the clarifications provided in the more recently published
        // https://w3c.github.io/json-ld-api/#node-map-generation

        // 1)
        // If element is an array, process each item in element as follows and then return:
        if (element.is_array()) {
            // 1.1)
            // Run this algorithm recursively by passing item for element, node map, active
            // graph, active subject, active property, and list.
            for (auto item : element) {
                generateNodeMap(item, nodeMap, blankNodeNames, activeGraph, activeSubject, activeProperty, list);
            }
            return;
        }

        // 2)
        // Otherwise element is a map. Reference the map which is the value of the active graph
        // entry of node map using the variable graph. If the active subject is null or a
        // map, set subject node to null otherwise reference the active subject entry of graph
        // using the variable subject node.
        if (!nodeMap.contains(*activeGraph)) {
            nodeMap[*activeGraph] = json::object();
        }
        json & graph = nodeMap[*activeGraph];

        json * subjectNode = nullptr;
        if(activeSubject == nullptr || activeSubject->is_object()) {
            subjectNode = nullptr;
        }
        else if (activeSubject->is_string() &&
                 graph.contains(activeSubject->get<std::string>())) {
            subjectNode = &graph[activeSubject->get<std::string>()];
        }

        // 3)
        // For each item in the @type entry of element, if any, or for the value of
        // @type, if the value of @type exists and is not an array:
        if (element.contains(JsonLdConsts::TYPE)) {
            // 3.1)
            // If item is a blank node identifier, replace it with a newly generated blank
            // node identifier passing item for identifier.
            json oldTypes;
            json newTypes;
            oldTypes = element[JsonLdConsts::TYPE];
            for (const auto& item : oldTypes) {
                std::string s = item.get<std::string>();
                if (s.find_first_of("_:") == 0) {
                    newTypes.push_back(blankNodeNames.get(s));
                } else {
                    newTypes.push_back(item);
                }
            }
            if(!newTypes.empty()) {
                if (element[JsonLdConsts::TYPE].is_array())
                    element[JsonLdConsts::TYPE] = newTypes;
                else
                    element[JsonLdConsts::TYPE] = newTypes.front();
            }
        }

        // 4)
        // If element has an @value entry, perform the following steps:
        if (element.contains(JsonLdConsts::VALUE)) {
            // 4.1)
            // If list is null:
            if (list == nullptr) {
                // 4.1.1)
                // If subject node (which must necessarily be a map) does not have an active
                // property entry, create one and initialize its value to an array containing element.
                // 4.1.2)
                // Otherwise, compare element against every item in the array associated with
                // the active property entry of subject node. If there is no item equivalent
                // to element, append element to the array. Two maps are considered equal if
                // they have equivalent map entries.
                JsonLdUtils::mergeValue(*subjectNode, *activeProperty, element);
            }
                // 4.2)
                // Otherwise, append element to the @list entry of list.
            else {
                list->at(JsonLdConsts::LIST).push_back(element);
            }
        }

            // 5)
            // Otherwise, if element has an @list entry, perform the following steps:
        else if (element.contains(JsonLdConsts::LIST)) {
            // 5.1)
            // Initialize a new map result consisting of a single entry @list whose value
            // is initialized to an empty array.
            json result = { { JsonLdConsts::LIST, json::array() } };
            // 5.2)
            // Recursively call this algorithm passing the value of element's @list entry for
            // element, node map, active graph, active subject, active property, and result for list.
            generateNodeMap(element[JsonLdConsts::LIST], nodeMap, blankNodeNames, activeGraph, activeSubject,
                            activeProperty, &result);
            // 5.3)
            // If list is null, append result to the value of the active property entry of subject
            // node (which must necessarily be a map).
            if (list == nullptr) {
                JsonLdUtils::mergeValue(*subjectNode, *activeProperty, result);
            }
                // 5.4)
                // Otherwise, append result to the @list entry of list.
            else {
                list->at(JsonLdConsts::LIST).push_back(result);
            }
        }

            // 6)
            // Otherwise element is a node object, perform the following steps:
        else {
            // 6.1)
            // If element has an @id entry, set id to its value and remove the entry from
            // element. If id is a blank node identifier, replace it with a newly generated
            // blank node identifier passing id for identifier.
            std::string id;
            if(element.contains(JsonLdConsts::ID)) {
                id = element[JsonLdConsts::ID].get<std::string>();
                element.erase(JsonLdConsts::ID);
                if (BlankNodeNames::hasFormOfBlankNodeName(id)) {
                    id = blankNodeNames.get(id);
                }
            }
                // 6.2)
                // Otherwise, set id to the result of the Generate Blank Node Identifier algorithm
                // passing null for identifier.
            else {
                id = blankNodeNames.get();
            }
            // 6.3)
            // If graph does not contain an entry id, create one and initialize its value to
            // a map consisting of a single entry @id whose value is id.
            if (!graph.contains(id)) {
                json tmp = { { JsonLdConsts::ID, id } };
                graph[id] = tmp;
                // knowing the insertion order comes in handy later in RDFDataset::graphToRDF()
                graph["key_insertion_order"].push_back(id);
            }
            // 6.4)
            // Reference the value of the id entry of graph using the variable node.
            json &node = graph[id];

            // 6.5)
            // If active subject is a map, a reverse property relationship is being
            // processed. Perform the following steps:
            if (activeSubject != nullptr && activeSubject->is_object()) {
                // 6.5.1)
                // If node does not have a active property entry, create one and initialize its
                // value to an array containing active subject.
                // 6.5.2
                // Otherwise, compare active subject against every item in the array associated
                // with the active property entry of node. If there is no item equivalent to
                // active subject, append active subject to the array. Two maps are considered
                // equal if they have equivalent map entries.
                JsonLdUtils::mergeValue(node, *activeProperty, *activeSubject);
            }
                // 6.6)
                // Otherwise, if active property is not null, perform the following steps:
            else if (activeProperty != nullptr) {
                // 6.6.1)
                // Create a new map reference consisting of a single entry @id whose value is id.
                json reference = { { JsonLdConsts::ID, id } };
                // 6.6.2)
                // If list is null:
                if (list == nullptr) {
                    // 6.6.2.1)
                    // If subject node does not have an active property entry, create one and
                    // initialize its value to an array containing reference.
                    // 6.6.2.2)
                    // Otherwise, compare reference against every item in the array associated
                    // with the active property entry of subject node. If there is no item
                    // equivalent to reference, append reference to the array. Two maps are
                    // considered equal if they have equivalent map entries.
                    JsonLdUtils::mergeValue(*subjectNode, *activeProperty, reference);
                }
                    // 6.6.3)
                    // Otherwise, append reference to the @list entry of list.
                else {
                    list->at(JsonLdConsts::LIST).push_back(reference);
                }
            }
            // 6.7)
            // If element has an @type entry, append each item of its associated array to the array
            // associated with the @type entry of node unless it is already in that array. Finally
            // remove the @type entry from element.
            if (element.contains(JsonLdConsts::TYPE)) {
                for (const auto& type : element[JsonLdConsts::TYPE]) {
                    JsonLdUtils::mergeValue(node, JsonLdConsts::TYPE, type);
                }
                element.erase(JsonLdConsts::TYPE);
            }
            // 6.8)
            // If element has an @index entry, set the @index entry of node to its value. If node
            // already has an @index entry with a different value, a conflicting indexes error has
            // been detected and processing is aborted. Otherwise, continue by removing the @index
            // entry from element.
            if (element.contains(JsonLdConsts::INDEX)) {
                json elemIndex = element[JsonLdConsts::INDEX];
                if (node.contains(JsonLdConsts::INDEX)) {
                    if (!JsonLdUtils::deepCompare(node.at(JsonLdConsts::INDEX), elemIndex)) {
                        throw JsonLdError(JsonLdError::ConflictingIndexes);
                    }
                }
                node[JsonLdConsts::INDEX] = elemIndex;
                element.erase(JsonLdConsts::INDEX);
            }
            // 6.9)
            // If element has an @reverse entry:
            if (element.contains(JsonLdConsts::REVERSE)) {
                // 6.9.1)
                // Create a map referenced node with a single entry @id whose value is id.
                json referencedNode = { { JsonLdConsts::ID, id } };
                // 6.9.2)
                // Initialize reverse map to the value of the @reverse entry of element.
                json reverseMap = element[JsonLdConsts::REVERSE];
                // 6.9.3)
                // For each key-value pair property-values in reverse map:
                std::vector<std::string> reverseMap_keys;
                for (json::iterator it = reverseMap.begin(); it != reverseMap.end(); ++it) {
                    reverseMap_keys.push_back(it.key());
                }
                for (auto property : reverseMap_keys) {
                    json values = reverseMap[property];
                    // 6.9.3.1)
                    // For each value of values:
                    for (auto reverseMap_value : values) {
                        // 6.9.3.1.1)
                        // Recursively invoke this algorithm passing value for element, node
                        // map, active graph, referenced node for active subject, and property
                        // for active property. Passing a map for active subject indicates to
                        // the algorithm that a reverse property relationship is being processed.
                        generateNodeMap(reverseMap_value, nodeMap, blankNodeNames, activeGraph,
                                        &referencedNode, &property, nullptr);
                    }
                }
                // 6.9.4)
                // Remove the @reverse entry from element.
                element.erase(JsonLdConsts::REVERSE);
            }
            // 6.10)
            // If element has an @graph entry, recursively invoke this algorithm passing the value
            // of the @graph entry for element, node map, and id for active graph before removing
            // the @graph entry from element.
            if (element.contains(JsonLdConsts::GRAPH)) {
                json elemGraph = element[JsonLdConsts::GRAPH];
                generateNodeMap(elemGraph, nodeMap, blankNodeNames, &id, nullptr, nullptr, nullptr);
                element.erase(JsonLdConsts::GRAPH);
            }
            // 6.11)
            // If element has an @included entry, recursively invoke this algorithm passing
            // the value of the @included entry for element, node map, and active graph before
            // removing the @included entry from element.
            if (element.contains(JsonLdConsts::INCLUDED)) {
                json elemIncluded = element[JsonLdConsts::INCLUDED];
                generateNodeMap(elemIncluded, nodeMap, blankNodeNames, activeGraph, nullptr, nullptr, nullptr);
                element.erase(JsonLdConsts::INCLUDED);
            }
            // 6.12)
            // Finally, for each key-value pair property-value in element ordered by property
            // perform the following steps:
            std::vector<std::string> keys;
            for (json::iterator it = element.begin(); it != element.end(); ++it) {
                keys.push_back(it.key());
            }
            std::sort(keys.begin(), keys.end());
            for (auto property : keys) {
                json & propertyValue = element[property];
                // 6.12.1)
                // If property is a blank node identifier, replace it with a newly generated
                // blank node identifier passing property for identifier.
                if (BlankNodeNames::hasFormOfBlankNodeName(property)) {
                    property = blankNodeNames.get(property);
                }
                // 6.12.2)
                // If node does not have a property entry, create one and initialize its value
                // to an empty array.
                if (!node.contains(property)) {
                    node[property] = json::array();
                }
                // 6.12.3)
                // Recursively invoke this algorithm passing value for element, node map, active
                // graph, id for active subject, and property for active property.
                json jid = id;
                generateNodeMap(propertyValue, nodeMap, blankNodeNames, activeGraph, &jid, &property, nullptr);
            }
        }
    }

    void generateNodeMap(json & element, json & nodeMap, BlankNodeNames &blankNodeNames)
    {
        std::string defaultGraph(JsonLdConsts::DEFAULT);
        generateNodeMap(element, nodeMap, blankNodeNames, &defaultGraph, nullptr, nullptr, nullptr);
    }

    std::shared_ptr<RDF::Node> objectToRDF(const nlohmann::json & item, RDFGraph & listTriples, const JsonLdOptions &options, BlankNodeNames &blankNodeNames);

    std::shared_ptr<RDF::Node> listToRDF(const nlohmann::json & list, RDFGraph & listTriples, const JsonLdOptions &options, BlankNodeNames &blankNodeNames) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the List to RDF conversion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#list-to-rdf-conversion

        // pre) The algorithm takes two inputs: an array list and an empty array list
        // triples used for returning the generated triples.
        assert(list.is_array());
        assert(listTriples.empty());

        // 1) If list is empty, return rdf:nil.
        if(list.empty())
            return std::make_shared<IRI>(JsonLdConsts::RDF_NIL);

        // 2) Otherwise, create an array bnodes composed of a newly generated blank node
        // identifier for each entry in list.
        std::vector<std::string> bnodes;
        //BlankNodeNames dummy;
        for(const auto& e : list)
            //bnodes.push_back(blankNodeNames->get(e)); -> 84 test failures
            //bnodes.push_back(dummy.get());            -> 77 test failures
            bnodes.push_back(blankNodeNames.get()); // -> 61 test failures


        // 3) For each pair of subject from bnodes and item from list:
        for (json::size_type index=0; index < list.size(); index++) {
            std::string subject = bnodes[index];
            json item = list[index];

            // 3.1)
            // Initialize embedded triples to a new empty array.
            RDFGraph embeddedTriples;

            // 3.2) Initialize object to the result of using the Object to RDF Conversion
            // algorithm passing item and embedded triples for list triples.
            std::shared_ptr<RDF::Node> object = objectToRDF(item, embeddedTriples, options, blankNodeNames);

            // 3.3)
            // Unless object is null, append a triple composed of subject, rdf:first, and object
            // to list triples.
            if(object) {
                std::shared_ptr<Node> s = std::make_shared<BlankNode>(subject);
                std::shared_ptr<Node> p = std::make_shared<IRI>(JsonLdConsts::RDF_FIRST);
                listTriples.emplace_back(s, p, object);
            }

            // 3.4)
            // Initialize rest as the next entry in bnodes, or if that does not
            // exist, rdf:nil. Append a triple composed of subject, rdf:rest, and rest
            // to list triples.
            std::string rest;
            if(index+1 < list.size()) {
                std::shared_ptr<Node> s = std::make_shared<BlankNode>(subject);
                std::shared_ptr<Node> p = std::make_shared<IRI>(JsonLdConsts::RDF_REST);
                std::shared_ptr<Node> o = std::make_shared<BlankNode>(bnodes[index+1]);
                listTriples.emplace_back(s, p, o);
            }
            else {
                std::shared_ptr<Node> s = std::make_shared<BlankNode>(subject);
                std::shared_ptr<Node> p = std::make_shared<IRI>(JsonLdConsts::RDF_REST);
                std::shared_ptr<Node> o = std::make_shared<IRI>(JsonLdConsts::RDF_NIL);
                listTriples.emplace_back(s, p, o);
            }

            // 3.5) Append all values from embedded triples to list triples
            for(const auto& t : embeddedTriples)
                listTriples.push_back(t);
        }

        // 4)
        // Return the first blank node from bnodes or rdf:nil if bnodes is empty.
        if(!bnodes.empty())
            return std::make_shared<BlankNode>(bnodes.front());
        else
            return std::make_shared<IRI>(JsonLdConsts::RDF_NIL);
    }

    std::shared_ptr<RDF::Node> objectToRDF(const nlohmann::json & item, RDFGraph & listTriples, const JsonLdOptions &options, BlankNodeNames &blankNodeNames) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Object to RDF conversion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#object-to-rdf-conversion

        json result;

        // pre) The algorithm takes as two arguments item which MUST be either a value
        // object, list object, or node object and list triples, which is an empty array.
        // Note: It also seems to be valid for item to be a string, which the spec does not mention?
        assert(JsonLdUtils::isObject(item) || JsonLdUtils::isListObject(item) || JsonLdUtils::isValueObject(item) || item.is_string());
        assert(listTriples.empty());

        // Note: It also seems to be valid for item to be a string, which the spec does not mention?
        if(item.is_string()) {
            std::string i = item.get<std::string>();
            if (i.find_first_of("_:") == 0) {
                return std::make_shared<BlankNode>(i);
            } else {
                return std::make_shared<IRI>(i);
            }
        }

        // 1)
        // If item is a node object and the value of its @id entry is not well-formed, return null.
        // todo: better detection of node object
        if (JsonLdUtils::isObject(item) && !JsonLdUtils::isListObject(item) && !JsonLdUtils::isValueObject(item)) {
            std::string id = item[JsonLdConsts::ID].get<std::string>();
            if (!BlankNodeNames::hasFormOfBlankNodeName(id) && JsonLdUtils::isRelativeIri(id)) {
                return nullptr;
            }
            // 2)
            // If item is a node object, return the IRI or blank node identifier associated with its @id entry.
            //id = item; // todo: should be item[JsonLdConsts::ID] ?
            if (id.find_first_of("_:") == 0) {
                return std::make_shared<BlankNode>(id);
            } else {
                return std::make_shared<IRI>(id);
            }
        }


        // 3)
        // If item is a list object return the result of the List Conversion algorithm, passing
        // the value associated with the @list entry from item and list triples.
        if (JsonLdUtils::isListObject(item)) {
            return listToRDF(item[JsonLdConsts::LIST], listTriples, options, blankNodeNames);
        }

        // 4)
        // Otherwise, item is a value object. Initialize value to the value associated with
        // the @value entry in item.
        assert(JsonLdUtils::isValueObject(item));

        json value = item["@value"];

        // 5)
        // Initialize datatype to the value associated with the @type entry of item or null if
        // item does not have such an entry.
        json datatype;
        if(item.contains(JsonLdConsts::TYPE))
            datatype = item[JsonLdConsts::TYPE];
        std::string datatypeStr;
        if (!datatype.is_null())
            datatypeStr = datatype.get<std::string>();

        // 6)
        // If datatype is not null and neither a well-formed IRI nor @json, return null.
        if(!datatype.is_null() && datatypeStr != JsonLdConsts::JSON && !JsonLdUtils::isAbsoluteIri(datatype.get<std::string>()))
            return nullptr;

        // 7)
        // If item has an @language entry which is not well-formed, return null.
        // todo: need well-formed language test
        if(item.contains(JsonLdConsts::LANGUAGE) && item[JsonLdConsts::LANGUAGE].empty())
            return nullptr;

        // 8)
        // If datatype is @json, convert value to the canonical lexical form using the result
        // of transforming the internal representation of value to JSON and set datatype to rdf:JSON.
        if(datatypeStr == JsonLdConsts::JSON) {
            throw JsonLdError(JsonLdError::NotImplemented, "need to implement @json handling");
        }

        // 9)
        // If value is true or false, set value to the string true or false which is the
        // canonical lexical form as described in § 8.6 Data Round Tripping If datatype is
        // null, set datatype to xsd:boolean.
        if (value.is_boolean()) {
            if (datatype.is_null())
                datatypeStr = JsonLdConsts::XSD_BOOLEAN;
            bool b = value.get<bool>();
            std::stringstream ss;
            ss << std::boolalpha << b;
            return std::make_shared<Literal>(
                    ss.str(),
                    &datatypeStr,
                    nullptr);
        }

        else if (value.is_number()) {
            // 10)
            // Otherwise, if value is a number with a non-zero fractional part (the result of a
            // modulo‑1 operation) or an absolute value greater or equal to 10^21, or value is a
            // number and datatype equals xsd:double, convert value to a string in canonical
            // lexical form of an xsd:double as defined in [XMLSCHEMA11-2] and described in
            // § 8.6 Data Round Tripping. If datatype is null, set datatype to xsd:double.
            if (value.is_number_float() || datatype == JsonLdConsts::XSD_DOUBLE) {
                if (datatype.is_null())
                    datatypeStr = JsonLdConsts::XSD_DOUBLE;
                double d = value.get<double>();
                return std::make_shared<Literal>(
                        DoubleFormatter::format(d),
                        &datatypeStr,
                        nullptr);
            } else {
                // 11)
                // Otherwise, if value is a number, convert it to a string in canonical lexical form of
                // an xsd:integer as defined in [XMLSCHEMA11-2] and described in § 8.6 Data Round
                // Tripping. If datatype is null, set datatype to xsd:integer.
                if (datatype.is_null())
                    datatypeStr = JsonLdConsts::XSD_INTEGER;
                int i = value.get<int>();
                return std::make_shared<Literal>(
                        std::to_string(i),
                        &datatypeStr,
                        nullptr);
            }
        }

            // 12)
            // Otherwise, if datatype is null, set datatype to xsd:string or rdf:langString, depending on if item has an @language entry.
        else if (datatype.is_null()) {
            if(item.contains(JsonLdConsts::LANGUAGE))
                datatypeStr = JsonLdConsts::RDF_LANGSTRING;
            else
                datatypeStr = JsonLdConsts::XSD_STRING;
        }
        // 13)
        // If item contains an @direction entry and rdfDirection is not null, item is a value object which is serialized using special rules.
        if(item.contains(JsonLdConsts::DIRECTION) && !options.getRdfDirection().empty()) {

            throw JsonLdError(JsonLdError::NotImplemented, "need to implement @direction handling");

            // 13.1)
            //  Initialize language to the value of @language in item normalized to lower case, or the empty string ("") if there is no such entry.

            // 13.2)
            // If rdfDirection is i18n-datatype, set datatype to the result of appending language and the value of @direction in item separated by an underscore ("_") to https://www.w3.org/ns/i18n#. Initialize literal as an RDF literal using value and datatype.

            // 13.3)
            // Otherwise, if rdfDirection is compound-literal:

            // 13.3.1)
            // Initialize literal as a new blank node.

            // 13.3.2)
            // Create a new triple using literal as the subject, rdf:value as the predicate, and the value of @value in item as the object, and add it to list triples.

            // 13.3.3)
            // If the item has an entry for @language, create a new triple using literal as the subject, rdf:language as the predicate, and language as the object, and add it to list triples.

            // 13.3.4)
            // Create a new triple using literal as the subject, rdf:direction as the predicate, and the value of @direction in item as the object, and add it to list triples.

        }

        // 14)
        // Otherwise, initialize literal as an RDF literal using value and datatype. If item
        // has an @language entry, add the value associated with the @language entry as the
        // language tag of literal.
        // 15)
        // Return literal.
        if(item.contains(JsonLdConsts::LANGUAGE)) {
            std::string languageStr = item["@language"].get<std::string>();
            return std::make_shared<Literal>(
                    value.get<std::string>(),
                    &datatypeStr,
                    &languageStr);
        }
        else {
            return std::make_shared<Literal>(
                    value.get<std::string>(),
                    &datatypeStr,
                    nullptr);
        }

    }

    void graphToRDF(const std::string &graphName, const json & graph, RDF::RDFDataset &dataset, BlankNodeNames & blankNodeNames, const JsonLdOptions &options) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Deserialize JSON-LD to RDF algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#deserialize-json-ld-to-rdf-algorithm

        // todo: remove?
        std::shared_ptr<Node> rdf_first = std::make_shared<IRI>(JsonLdConsts::RDF_FIRST);
        std::shared_ptr<Node> rdf_rest = std::make_shared<IRI>(JsonLdConsts::RDF_REST);
        std::shared_ptr<Node> rdf_nil = std::make_shared<IRI>(JsonLdConsts::RDF_NIL);

        // 1.1)
        // If graph name is not well-formed, continue with the next graph name-graph pair.
        // todo: better well-formed checking
        if(graphName.empty())
            return;

        // 1.2)
        // If graph name is @default, initialize triples to the value of the defaultGraph
        // attribute of dataset. Otherwise, initialize triples as an empty RdfGraph and add
        // to dataset using its add method along with graph name for graphName.
        RDFGraph triples = dataset.getGraph(graphName); // will add later using setGraph()

        // 1.3)
        // For each subject and node in graph ordered by subject:
        // todo: ordered by subject? not insertion order?
        std::vector<std::string> subjects;
        if(graph.contains("key_insertion_order")) {
            for (const auto & it : graph["key_insertion_order"]) {
                subjects.push_back(it.get<std::string>());
            }
        }
        for (const auto& subject : subjects) {
            const json &node = graph[subject];

            // 1.3.1)
            // If subject is not well-formed, continue with the next subject-node pair.
            // todo: better well-formed checking
            if (!BlankNodeNames::hasFormOfBlankNodeName(subject) && JsonLdUtils::isRelativeIri(subject)) {
                continue;
            }

            // 1.3.2)
            // For each property and values in node ordered by property:
            std::vector<std::string> properties;
            for (json::const_iterator it = node.begin(); it != node.end(); ++it) {
                properties.push_back(it.key());
            }
            std::sort(properties.begin(), properties.end());

            for (auto property : properties) {

                const json * values;

                // 1.3.2.1)
                // If property is @type, then for each type in values, create a new RdfTriple
                // composed of subject, rdf:type for predicate, and type for object and add to
                // triples using its add method, unless type is not well-formed.
                if (property == JsonLdConsts::TYPE) {
                    values = &node[JsonLdConsts::TYPE];
                    property = JsonLdConsts::RDF_TYPE;
                }

                    // 1.3.2.2)
                    // Otherwise, if property is a keyword continue with the next property-values pair.
                else if (JsonLdUtils::isKeyword(property)) {
                    continue;
                }

                    // 1.3.2.3)
                    // Otherwise, if property is a blank node identifier and the produceGeneralizedRdf
                    // option is not true, continue with the next property-values pair.
                else if (::BlankNodeNames::hasFormOfBlankNodeName(property) && !options.getProduceGeneralizedRdf()) {
                    continue;
                }

                    // 1.3.2.4)
                    // Otherwise, if property is not well-formed, continue with the next property-values pair.
                else if (JsonLdUtils::isRelativeIri(property)) {
                    continue;
                } else {
                    values = &node[property];
                }

                // 1.3.2.5)
                // Otherwise, property is an IRI or blank node identifier. For each item in values:
                for (const auto& item : *values) {

                    // 1.3.2.5.1)
                    // Initialize list triples as an empty array.
                    RDFGraph listTriples;

                    // 1.3.2.5.2)
                    // Add a triple composed of subject, property, and the result of using the
                    // Object to RDF Conversion algorithm passing item and list triples to triples
                    // using its add method, unless the result is null, indicating a non-well-formed
                    // resource that has to be ignored.
                    auto result = objectToRDF(item, listTriples, options, blankNodeNames);
                    if(result) {
                        std::shared_ptr<Node> s;
                        if (::BlankNodeNames::hasFormOfBlankNodeName(subject))
                            s = std::make_shared<BlankNode>(subject);
                        else
                            s = std::make_shared<IRI>(subject);

                        std::shared_ptr<Node> p;
                        if (::BlankNodeNames::hasFormOfBlankNodeName(property))
                            p = std::make_shared<BlankNode>(property);
                        else
                            p = std::make_shared<IRI>(property);

                        triples.emplace_back(s, p, result);
                    }

                    // 1.3.2.5.3)
                    // Add all RdfTriple instances from list triples to triples using its add method.
                    for(const auto& t : listTriples)
                        triples.push_back(t);

                }
            }
        }

        // write triples back to the dataset
        dataset.setGraph(triples, graphName);

    }


    RDF::RDFDataset
    toRDF(nlohmann::json nodeMap, BlankNodeNames & blankNodeNames, const JsonLdOptions &options) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Deserialize JSON-LD to RDF algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#deserialize-json-ld-to-rdf-algorithm

        RDF::RDFDataset dataset(options);

        // 1)
        // For each graph name and graph in node map ordered by graph name:
        // todo: ordered?
        for (json::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
            auto & graphName = static_cast<const std::string &>(it.key());
            json & graph = it.value();

            graphToRDF(graphName, graph, dataset, blankNodeNames, options);
        }

        return dataset;
    }

}

RDF::RDFDataset RDFSerializationProcessor::toRDF(nlohmann::json expandedInput, const JsonLdOptions& options) {

    // Comments in this function are labeled with numbers that correspond to sections
    // from the description of the deserialize JSON-LD to RDF algorithm.
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-tordf


    // See: https://www.w3.org/TR/json-ld11-api/#deserialize-json-ld-to-rdf-algorithm

    // 3)
    // Create a new RdfDataset dataset.
    //RDF::RDFDataset dataset(options);

    // 4)
    // Create a new map node map.
    auto nodeMap = json::object();
    nodeMap[JsonLdConsts::DEFAULT] = json::object();

    // 5)
    // Invoke the Node Map Generation algorithm, passing expanded input as element and node map.
    BlankNodeNames blankNodeNames;
    generateNodeMap(expandedInput, nodeMap, blankNodeNames);

    // 6)
    // Invoke the Deserialize JSON-LD to RDF Algorithm passing node map, dataset, and the
    // produceGeneralizedRdf flag from options.
    //dataset.toRDF(nodeMap);
    RDF::RDFDataset dataset = ::toRDF(nodeMap, blankNodeNames, options);

    // 7)
    // Resolve the promise with dataset.
    return dataset;
}

