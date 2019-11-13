#include "JsonLdApi.h"
#include "ObjUtils.h"
#include "NormalizeUtils.h"

#include <utility>
#include <iostream>
#include <set>

using nlohmann::json;

JsonLdApi::JsonLdApi(JsonLdOptions ioptions)
        : options(std::move(ioptions)) {
}

JsonLdOptions JsonLdApi::getOptions() const {
    return options;
}

json JsonLdApi::expand(Context activeCtx, json element) {
    return expand(std::move(activeCtx), nullptr, std::move(element));
}

json JsonLdApi::expand(Context activeCtx, std::string * activeProperty, json element) {

    // 1)
    if (element.empty()) {
        return element; // todo: was null...
    }

    if(element.is_array())
        return expandArrayElement(activeCtx, activeProperty, element);
    else if(element.is_object())
        return expandObjectElement(activeCtx, activeProperty, element);
    else // scalar
    {
        // 2.1)
        if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
            return json(); // todo: was null
        }
        return activeCtx.expandValue(*activeProperty, element);
    }
}

json JsonLdApi::expandArrayElement(Context activeCtx, std::string * activeProperty, const json& element) {
    // 3.1)
    json result = json::array();
    // 3.2)
    for (auto &item : element) {
        // 3.2.1)
        json v = expand(activeCtx, activeProperty, item);
        // 3.2.2)
        if(activeProperty != nullptr) {
            if ((*activeProperty == JsonLdConsts::LIST
                 || activeCtx.getContainer(*activeProperty) == JsonLdConsts::LIST)
                && (v.is_array() || (v.is_object() && v.contains(JsonLdConsts::LIST)))) {
                throw JsonLdError(JsonLdError::ListOfLists, "lists of lists are not permitted.");
            }
        }
        // 3.2.3)
        if (!v.is_null()) { // != null
            if (v.is_array()) {
                result.insert(result.end(), v.begin(), v.end());
            } else {
                result.push_back(v);
            }
        }
    }
    // 3.3)
    return result;
}

json JsonLdApi::expandObjectElement(Context activeCtx, std::string * activeProperty, json element) {

    // access helper
    // 5)
    if (element.contains(JsonLdConsts::CONTEXT)) {
        activeCtx = activeCtx.parse(element[JsonLdConsts::CONTEXT]);
    }
    // 6)
    json result = ObjUtils::newMap();
    // 7)
    std::vector<std::string> element_keys;
    for (json::iterator it = element.begin(); it != element.end(); ++it) {
        element_keys.push_back(it.key());
    }
    std::sort(element_keys.begin(), element_keys.end());
    for (auto & key : element_keys) {
        auto element_value = element[key];
        // 7.1)
        if (key == JsonLdConsts::CONTEXT) {
            continue;
        }
        // 7.2)
        std::string expandedProperty = activeCtx.expandIri(key, false, true);
        json expandedValue;
        // 7.3)
        if (
                // expandedProperty == nullptr ||
                expandedProperty.empty() ||
                (expandedProperty.find(':') == std::string::npos && !JsonLdUtils::isKeyword(expandedProperty))) {
            continue;
        }
        // 7.4)
        if (JsonLdUtils::isKeyword(expandedProperty)) {
            // 7.4.1)
            if (activeProperty != nullptr && *activeProperty == JsonLdConsts::REVERSE) {
                throw JsonLdError(JsonLdError::InvalidReversePropertyMap,
                                      "a keyword cannot be used as a @reverse property");
            }
            // 7.4.2)
            if (result.contains(expandedProperty)) {
                throw JsonLdError(JsonLdError::CollidingKeywords,
                                      expandedProperty + " already exists in result");
            }
            // 7.4.3)
            if (expandedProperty == JsonLdConsts::ID) {
                if (element_value.is_string()) {
                    expandedValue = activeCtx.expandIri(element_value.get<std::string>(), true, false);
                } else if (options.getFrameExpansion()) {
                    if (element_value.is_object()) {
                        if (!element_value.empty()) {
                            throw JsonLdError(JsonLdError::InvalidIdValue,
                                              "@id value must be a an empty object for framing");
                        }
                        expandedValue = element_value;
                    } else if (element_value.is_array()) {
                        for ( const auto& v : element_value) {
                            if (!(v.is_string())) {
                                throw JsonLdError(JsonLdError::InvalidIdValue,
                                                  "@id value must be a string, an array of strings or an empty dictionary");
                            }
                            expandedValue.push_back(activeCtx.expandIri(v.get<std::string>(), true, true));
                        }
                    } else {
                        throw JsonLdError(JsonLdError::InvalidIdValue,
                                          "value of @id must be a string, an array of strings or an empty dictionary");
                    }
                } else {
                    throw JsonLdError(JsonLdError::InvalidIdValue,
                                      "value of @id must be a string");
                }
            }
                // 7.4.4)
            else if (expandedProperty == JsonLdConsts::TYPE) {
                if (element_value.is_array()) {
                    for ( const auto& v : element_value) {
                        if (!(v.is_string())) {
                            throw JsonLdError(JsonLdError::InvalidTypeValue,
                                              "@type value must be a string or array of strings");
                        }
                        expandedValue.push_back(
                                activeCtx.expandIri(v.get<std::string>(), true, true));
                    }
                } else if (element_value.is_string()) {
                    expandedValue = activeCtx.expandIri(element_value.get<std::string>(), true, true);
                }
                // TODO: SPEC: no mention of empty map check
                else if (options.getFrameExpansion() && element_value.is_object()) {
                    if (!element_value.empty()) {
                        throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a an empty object for framing");
                    }
                    expandedValue = element_value;
                } else {
                    throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a string or array of strings");
                }
            }
                // 7.4.5)
            else if (expandedProperty == JsonLdConsts::GRAPH) {
                std::string currentProperty = JsonLdConsts::GRAPH;
                expandedValue = expand(activeCtx, &currentProperty, element_value);
            }
                // 7.4.6)
            else if (expandedProperty == JsonLdConsts::VALUE) {
                if (!element_value.is_null() && (element_value.is_object() || element_value.is_array())) {
                    throw JsonLdError(JsonLdError::InvalidValueObjectValue,
                                          "value of " + expandedProperty + " must be a scalar or null");
                }
                expandedValue = element_value;
                if (expandedValue.is_null()) {
                    result[JsonLdConsts::VALUE] = expandedValue; // was null
                    continue;
                }
            }
                // 7.4.7)
            else if (expandedProperty == JsonLdConsts::LANGUAGE) {
                if (!(element_value.is_string())) {
                    throw JsonLdError(JsonLdError::InvalidLanguageTaggedString,
                                          "Value of " + expandedProperty + " must be a string");
                }
                std::string v = element_value.get<std::string>();
                std::transform(v.begin(), v.end(), v.begin(), &::tolower);
                expandedValue = v;
            }
                // 7.4.8)
            else if (expandedProperty == JsonLdConsts::INDEX) {
                if (!(element_value.is_string())) {
                    throw JsonLdError(JsonLdError::InvalidIndexValue,
                                          "Value of " + expandedProperty + " must be a string");
                }
                expandedValue = element_value;
            }
                // 7.4.9)
            else if (expandedProperty == JsonLdConsts::LIST) {
                // 7.4.9.1)
                if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
                    continue;
                }
                // 7.4.9.2)
                expandedValue = expand(activeCtx, activeProperty, element_value);

                // NOTE: step not in the spec yet
                if (!(expandedValue.is_array())) {
                    json j;
                    j.push_back(expandedValue);
                    expandedValue = j;
                }

                // 7.4.9.3)
                for ( const auto& v : expandedValue) {
                    if (v.is_object() && v.contains(JsonLdConsts::LIST)) {
                        throw JsonLdError(JsonLdError::ListOfLists,
                                              "A list may not contain another list");
                    }
                }
            }
                // 7.4.10)
            else if (expandedProperty == JsonLdConsts::SET) {
                expandedValue = expand(activeCtx, activeProperty, element_value);
            }
                // 7.4.11)
            else if (expandedProperty == JsonLdConsts::REVERSE) {
                if (!(element_value.is_object())) {
                    throw JsonLdError(JsonLdError::InvalidReverseValue,
                                          "@reverse value must be an object");
                }
                // 7.4.11.1)
                {
                    std::string currentProperty = JsonLdConsts::REVERSE;
                    expandedValue = expand(activeCtx, &currentProperty, element_value);
                }
                // NOTE: algorithm assumes the result is a map
                // 7.4.11.2)
                if (expandedValue.contains(JsonLdConsts::REVERSE)) {
                    auto reverse = expandedValue[JsonLdConsts::REVERSE];
                    std::vector<std::string> reverse_keys;
                    for (json::iterator it = reverse.begin(); it != reverse.end(); ++it) {
                        reverse_keys.push_back(it.key());
                    }
                    for (const auto &property : reverse_keys) {
                        auto item = reverse[property];
                        // 7.4.11.2.1)
                        if (!result.contains(property)) {
                            result[property] = json::array();
                        }
                        // 7.4.11.2.2)
                        if (item.is_array()) {
                            result[property].insert(result[property].end(), item.begin(), item.end());
                        } else {
                            result[property] += item;
                        }
                    }
                }
                // 7.4.11.3)
                if (expandedValue.size() > (expandedValue.contains(JsonLdConsts::REVERSE) ? 1 : 0)) {
                    // 7.4.11.3.1)
                    if (!result.contains(JsonLdConsts::REVERSE)) {
                        result[JsonLdConsts::REVERSE] = json::object();
                    }
                    // 7.4.11.3.2)
                    auto & reverseMap = result[JsonLdConsts::REVERSE];
                    // 7.4.11.3.3)
                    std::vector<std::string> expandedValue_keys;
                    for (json::iterator it = expandedValue.begin(); it != expandedValue.end(); ++it) {
                        expandedValue_keys.push_back(it.key());
                    }
                    for ( const auto& property : expandedValue_keys) {
                        if (property == JsonLdConsts::REVERSE) {
                            continue;
                        }
                        // 7.4.11.3.3.1)
                        auto items = expandedValue[property];
                        for ( const auto& item : items) {
                            // 7.4.11.3.3.1.1)
                            if (item.is_object() && (item.contains(JsonLdConsts::VALUE)
                                || item.contains(JsonLdConsts::LIST))) {
                                throw JsonLdError(JsonLdError::InvalidReversePropertyValue);
                            }
                            // 7.4.11.3.3.1.2)
                            if (!reverseMap.contains(property)) {
                                reverseMap[property] = json::array();
                            }
                            // 7.4.11.3.3.1.3)
                            reverseMap[property] += item;
                        }
                    }
                }
                // 7.4.11.4)
                continue;
            }
                // TODO: SPEC no mention of @explicit etc in spec
            else if (options.getFrameExpansion() && (expandedProperty == JsonLdConsts::EXPLICIT
                                        || expandedProperty == JsonLdConsts::DEFAULT
                                        || expandedProperty == JsonLdConsts::EMBED
                                        || expandedProperty == JsonLdConsts::REQUIRE_ALL
                                        || expandedProperty == JsonLdConsts::EMBED_CHILDREN
                                        || expandedProperty == JsonLdConsts::OMIT_DEFAULT)) {
                expandedValue = expand(activeCtx, &expandedProperty, element_value);
            }
            // 7.4.12)
            if (!expandedValue.is_null()) {
                result[expandedProperty] = expandedValue;
            }
            // 7.4.13)
            continue;
        }
            // 7.5
        else if (activeCtx.getContainer(key) == JsonLdConsts::LANGUAGE && element_value.is_object()) {
            // 7.5.1)
            // 7.5.2)
            for(auto& el : element_value.items()) {
                std::string language = el.key();
                json languageValue = el.value();

                // 7.5.2.1)
                if (!(languageValue.is_array())) {
                    json j;
                    j.push_back(languageValue);
                    languageValue = j;
                }
                // 7.5.2.2)
                for ( const auto& item : languageValue) {
                    // 7.5.2.2.1)
                    if (!(item.is_string())) {
                        std::stringstream ss;
                        ss << "Expected: " << item << " to be a string";
                        throw JsonLdError(JsonLdError::InvalidLanguageMapValue, ss.str());
                    }
                    // 7.5.2.2.2)
                    json tmp = ObjUtils::newMap();
                    tmp[JsonLdConsts::VALUE] = item;
                    std::transform(language.begin(), language.end(), language.begin(), &::tolower);
                    tmp[JsonLdConsts::LANGUAGE] = language;
                    expandedValue.push_back(tmp);
                }
            }
        }
        // 7.6)
        else if (activeCtx.getContainer(key) == JsonLdConsts::INDEX
                 && element_value.is_object()) {
            // 7.6.1)
            // 7.6.2)
            std::vector<std::string> indexKeys;
            for(auto& el : element_value.items()) {
                indexKeys.push_back(el.key());
            }
            std::sort(indexKeys.begin(), indexKeys.end());
            for ( const auto& index : indexKeys) {
                json indexValue = element_value[index];
                // 7.6.2.1)
                if (!(indexValue.is_array())) {
                    json j;
                    j.push_back(indexValue);
                    indexValue = j;
                }
                // 7.6.2.2)
                indexValue = expand(activeCtx, &key, indexValue);
                // 7.6.2.3)
                for ( auto item : indexValue) {
                    // 7.6.2.3.1)
                    if (!item.contains(JsonLdConsts::INDEX)) {
                        item[JsonLdConsts::INDEX] = index;
                    }
                    // 7.6.2.3.2)
                    expandedValue.push_back(item);
                }
            }
        }
        // 7.7)
        else {
            expandedValue = expand(activeCtx, &key, element_value);
        }
        // 7.8)
        if (expandedValue.is_null()) {
            continue;
        }
        // 7.9)
        if (activeCtx.getContainer(key) == JsonLdConsts::LIST) {
            if (!expandedValue.is_object() || !expandedValue.contains(JsonLdConsts::LIST)) {
                json tmp = expandedValue;
                if (!tmp.is_array()) {
                    json j;
                    j.push_back(expandedValue);
                    tmp = j;
                }
                expandedValue = ObjUtils::newMap();
                expandedValue[JsonLdConsts::LIST] = tmp;
            }
        }
        // 7.10)
        if (activeCtx.isReverseProperty(key)) {
            // 7.10.1)
            if (!result.contains(JsonLdConsts::REVERSE)) {
                result[JsonLdConsts::REVERSE] = ObjUtils::newMap();
            }
            // 7.10.2)
             auto & reverseMap = result[JsonLdConsts::REVERSE];
            // 7.10.3)
            if (!(expandedValue.is_array())) {
                json j;
                j.push_back(expandedValue);
                expandedValue = j;
            }
            // 7.10.4)
            for ( auto item : expandedValue) {
                // 7.10.4.1)
                if (item.is_object() && (item.contains(JsonLdConsts::VALUE) || item.contains(JsonLdConsts::LIST))) {
                    throw JsonLdError(JsonLdError::InvalidReversePropertyValue);
                }
                // 7.10.4.2)
                if (!reverseMap.contains(expandedProperty)) {
                    reverseMap[expandedProperty] = json::array();
                }
                // 7.10.4.3)
                if (item.is_array()) {
                    reverseMap[expandedProperty].insert(reverseMap[expandedProperty].end(), item.begin(), item.end());
                } else {
                    reverseMap[expandedProperty] += item;
                }
            }
        }
            // 7.11)
        else {
            // 7.11.1)
            if (!result.contains(expandedProperty)) {
                result[expandedProperty] = json::array();
            }
            // 7.11.2)
            if (expandedValue.is_array()) {
                result[expandedProperty].insert(result[expandedProperty].end(), expandedValue.begin(), expandedValue.end());
            } else {
                result[expandedProperty].push_back(expandedValue);
            }
        }
    }
    // 8)
    if (result.contains(JsonLdConsts::VALUE)) {
        // 8.1)
        // TODO: is this method faster than just using contains for each?
        std::set<std::string> keySet;
        for(auto& el : result.items()) {
            keySet.insert(el.key());
        }
        keySet.erase(JsonLdConsts::VALUE);
        keySet.erase(JsonLdConsts::INDEX);
         bool langRemoved = keySet.erase(JsonLdConsts::LANGUAGE) > 0;
         bool typeRemoved = keySet.erase(JsonLdConsts::TYPE) > 0;
        if ((langRemoved && typeRemoved) || !keySet.empty()) {
            throw JsonLdError(JsonLdError::InvalidValueObject,
                                  "value object has unknown keys");
        }
        // 8.2)
        json rval = result[JsonLdConsts::VALUE];
        if (rval.is_null()) {
            // nothing else is possible with result if we set it to
            // null, so simply return it
            return rval;
        }
        // 8.3)
        if (!rval.is_string() && result.contains(JsonLdConsts::LANGUAGE)) {
            throw JsonLdError(JsonLdError::InvalidLanguageTaggedValue,
                                  "when @language is used, @value must be a string");
        }
        // 8.4)
        else if (result.contains(JsonLdConsts::TYPE)) {
            // TODO: is this enough for "is an IRI"
            json j = result[JsonLdConsts::TYPE];
            if (!j.is_string()) {
                throw JsonLdError(JsonLdError::InvalidTypedValue,
                                  "value of @type must be an IRI");
            }
            std::string jStr = j.get<std::string>();
            if(jStr.find("_:") == 0 || jStr.find(':') == std::string::npos) {
                throw JsonLdError(JsonLdError::InvalidTypedValue,
                                      "value of @type must be an IRI");
            }
        }
    }
        // 9)
    else if (result.contains(JsonLdConsts::TYPE)) {
         json rtype = result[JsonLdConsts::TYPE];
        if (!rtype.is_array()) {
            json j;
            j.push_back(rtype);
            result[JsonLdConsts::TYPE] = j;
        }
    }
        // 10)
    else if (result.contains(JsonLdConsts::SET) || result.contains(JsonLdConsts::LIST)) {
        // 10.1)
        if (result.size() > (result.contains(JsonLdConsts::INDEX) ? 2 : 1)) {
            throw JsonLdError(JsonLdError::InvalidSetOrListObject,
                                  "@set or @list may only contain @index");
        }
        // 10.2)
        if (result.contains(JsonLdConsts::SET)) {
            // result becomes an array here, thus the remaining checks
            // will never be true from here on
            // so simply return the value rather than have to make
            // result an object and cast it with every
            // other use in the function.
            return result[JsonLdConsts::SET];
        }
    }
    // 11)
    if (result.contains(JsonLdConsts::LANGUAGE) && result.size() == 1) {
        json j;
        result = j;
    }
    // 12)
    if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
        // 12.1)
        json j;
        if (!result.is_null() &&
            (result.empty() || result.contains(JsonLdConsts::VALUE) || result.contains(JsonLdConsts::LIST))) {
            result = j;
        }
            // 12.2)
        else if (!result.is_null() && !options.getFrameExpansion() && result.contains(JsonLdConsts::ID)
                 && result.size() == 1) {
            result = j;
        }
    }
    // 13)
    return result;
}

RDF::RDFDataset JsonLdApi::toRDF(nlohmann::json element) {
    auto nodeMap = ObjUtils::newMap();
    nodeMap[JsonLdConsts::DEFAULT] = ObjUtils::newMap();
    generateNodeMap(element, nodeMap);
    RDF::RDFDataset dataset(options, &blankNodeUniqueNamer);

    std::vector<std::string> keys;
    for (json::iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        keys.push_back(it.key());
    }

    for (auto & graphName : keys) {
        // 4.1)
        if (JsonLdUtils::isRelativeIri(graphName)) {
            continue;
        }
        json & graph = nodeMap[graphName];
        dataset.graphToRDF(graphName, graph);
    }

    return dataset;
}

void JsonLdApi::generateNodeMap(json & element, json &nodeMap, std::string *activeGraph, nlohmann::json *activeSubject,
                                std::string *activeProperty, json *list)
{
    // 1)
    if (element.is_array()) {
        // 1.1)
        for (auto item : element) {
            generateNodeMap(item, nodeMap, activeGraph, activeSubject, activeProperty, list);
        }
        return;
    }

    // 2)
    if (!nodeMap.contains(*activeGraph)) {
        nodeMap[*activeGraph] = ObjUtils::newMap();
    }
    json & graph = nodeMap[*activeGraph];

    json * node = nullptr;
    if (activeSubject != nullptr && activeSubject->is_string() &&
        graph.contains(activeSubject->get<std::string>())) {
        node = &graph[activeSubject->get<std::string>()];
    }

    // 3)
    if (element.contains(JsonLdConsts::TYPE)) {
        // 3.1)
        json oldTypes;
        json newTypes;
        oldTypes = element[JsonLdConsts::TYPE];
        for (const auto& item : oldTypes) {
            std::string s = item.get<std::string>();
            if (s.find_first_of("_:") == 0) {
                newTypes.push_back(blankNodeUniqueNamer.get(item));
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
    if (element.contains(JsonLdConsts::VALUE)) {
        // 4.1)
        if (list == nullptr) {
            // todo: seems like at this point there shouldn't ever be a way for activeProperty to be null, but we might want to check anyway
            if(activeProperty == nullptr)
                throw  std::runtime_error("activeProperty should not be nullptr");
            JsonLdUtils::mergeValue(*node, *activeProperty, element);
        }
            // 4.2)
        else {
            JsonLdUtils::mergeValue(*list, JsonLdConsts::LIST, element);
        }
    }

        // 5)
    else if (element.contains(JsonLdConsts::LIST)) {
        // 5.1)
        json result = ObjUtils::newMap(JsonLdConsts::LIST, json::array());
        // 5.2)
        generateNodeMap(element[JsonLdConsts::LIST], nodeMap, activeGraph, activeSubject,
                        activeProperty, &result);
        // 5.3)
        JsonLdUtils::mergeValue(*node, *activeProperty, result);
    }

    else {

        // 6.1)
        std::string id;
        if(element.contains(JsonLdConsts::ID)) {
            id = element[JsonLdConsts::ID];
            element.erase(JsonLdConsts::ID);
            if (id.find_first_of("_:") == 0) {
                id = blankNodeUniqueNamer.get(id);
            }
        }
        // 6.2)
        else {
            id = blankNodeUniqueNamer.get();
        }
        // 6.3)
        if (!graph.contains(id)) {
            json tmp = ObjUtils::newMap(JsonLdConsts::ID, id);
            graph[id] = tmp;
            graph["key_insertion_order"].push_back(id);
        }
        // 6.4) removed for now
        // 6.5)
        if (activeSubject != nullptr && activeSubject->is_object()) {
            // 6.5.1)
            JsonLdUtils::mergeValue(graph[id], *activeProperty, *activeSubject);
        }
            // 6.6)
        else if (activeProperty != nullptr) {
            json reference = ObjUtils::newMap(JsonLdConsts::ID, id);
            // 6.6.2)
            if (list == nullptr) {
                // 6.6.2.1+2)
                JsonLdUtils::mergeValue(*node, *activeProperty, reference);
            }
                // 6.6.3) TODO: SPEC says to add ELEMENT to @list member, should be REFERENCE
            else {
                JsonLdUtils::mergeValue(*list, JsonLdConsts::LIST, reference);
            }
        }
        // TODO: SPEC this is removed in the spec now, but it's still needed
        // (see 6.4)
        node = &graph[id];
        // 6.7)
        if (element.contains(JsonLdConsts::TYPE)) {
            json types = element[JsonLdConsts::TYPE];
            element.erase(JsonLdConsts::TYPE);
            for (const auto& type : types) {
                JsonLdUtils::mergeValue(*node, JsonLdConsts::TYPE, type);
            }
        }
        // 6.8)
        if (element.contains(JsonLdConsts::INDEX)) {
            json elemIndex = element[JsonLdConsts::INDEX];
            element.erase(JsonLdConsts::INDEX);
            if (node->contains(JsonLdConsts::INDEX)) {
                if (!JsonLdUtils::deepCompare(node->at(JsonLdConsts::INDEX), elemIndex)) {
                    throw JsonLdError(JsonLdError::ConflictingIndexes);
                }
            } else {
                (*node)[JsonLdConsts::INDEX] = elemIndex;
            }
        }
        // 6.9)
        if (element.contains(JsonLdConsts::REVERSE)) {
            // 6.9.1)
            json referencedNode = ObjUtils::newMap(JsonLdConsts::ID, id);
            // 6.9.2+6.9.4)
            json reverseMap = element[JsonLdConsts::REVERSE];
            element.erase(JsonLdConsts::REVERSE);
            // 6.9.3)
            std::vector<std::string> reverseMap_keys;
            for (json::iterator it = reverseMap.begin(); it != reverseMap.end(); ++it) {
                reverseMap_keys.push_back(it.key());
            }
            for (auto property : reverseMap_keys) {
                json values = reverseMap[property];
                // 6.9.3.1)
                for (auto reverseMap_value : values) {
                    // 6.9.3.1.1)
                    generateNodeMap(reverseMap_value, nodeMap, activeGraph, &referencedNode, &property, nullptr);
                }
            }
        }
        // 6.10)
        if (element.contains(JsonLdConsts::GRAPH)) {
            json elemGraph = element[JsonLdConsts::GRAPH];
            element.erase(JsonLdConsts::GRAPH);
            generateNodeMap(elemGraph, nodeMap, &id, nullptr, nullptr, nullptr);
        }
        // 6.11)
        std::vector<std::string> keys;
        for (json::iterator it = element.begin(); it != element.end(); ++it) {
            keys.push_back(it.key());
        }
        std::sort(keys.begin(), keys.end());
        for (auto property : keys) {
            json & propertyValue = element[property];
            // 6.11.1)
            if (property.find_first_of("_:") == 0) {
                property = blankNodeUniqueNamer.get(property);
            }
            // 6.11.2)
            if (node != nullptr && !node->contains(property)) {
                (*node)[property] = json::array();
            }
            // 6.11.3)
            json jid = id;
            generateNodeMap(propertyValue, nodeMap, activeGraph, &jid, &property, nullptr);
        }

    }

}

void JsonLdApi::generateNodeMap(json & element, json & nodeMap)
{
    std::string defaultGraph(JsonLdConsts::DEFAULT);
    generateNodeMap(element, nodeMap, &defaultGraph, nullptr, nullptr, nullptr);
}

std::string JsonLdApi::normalize(const RDF::RDFDataset& dataset) {
    // create quads and map bnodes to their associated quads
    std::vector<RDF::Quad> quads;
    std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> bnodes; //todo: this is a crazy data type
    std::vector<std::string> bnodes_insertion_order_keys;
    for (auto graphName : dataset.graphNames()) {
        std::vector<RDF::Quad> triples = dataset.getQuads(graphName);
        std::string *graphNamePtr = &graphName;
        if (graphName == "@default") {
            graphNamePtr = nullptr;
        }
        for (auto quad : triples) {
            if(graphNamePtr != nullptr) {
               quad.setGraph(graphNamePtr);
            }

            quads.push_back(quad);

            // todo: replace this with an iteration over function pointers
            auto n = quad.getSubject();
            if(n != nullptr) {
                if(n->isBlankNode()) {
                    std::string id = n->getValue();
                    if(!bnodes.count(id)) {
                        std::vector<RDF::Quad> tmp;
                        bnodes[id]["quads"] = tmp;
                        bnodes_insertion_order_keys.push_back(id);
                    }
                    bnodes[id]["quads"].push_back(quad);
                }
            }
            n = quad.getObject();
            if(n != nullptr) {
                if(n->isBlankNode()) {
                    std::string id = n->getValue();
                    if(!bnodes.count(id)) {
                        std::vector<RDF::Quad> tmp;
                        bnodes[id]["quads"] = tmp;
                        bnodes_insertion_order_keys.push_back(id);
                    }
                    bnodes[id]["quads"].push_back(quad);
                }
            }
            n = quad.getGraph();
            if(n != nullptr) {
                if(n->isBlankNode()) {
                    std::string id = n->getValue();
                    if(!bnodes.count(id)) {
                        std::vector<RDF::Quad> tmp;
                        bnodes[id]["quads"] = tmp;
                        bnodes_insertion_order_keys.push_back(id);
                    }
                    bnodes[id]["quads"].push_back(quad);
                }
            }
        }
    }

    // mapping complete, start canonical naming
    NormalizeUtils normalizeUtils(quads, bnodes, UniqueNamer("_:c14n"), options);
    std::vector<std::string> ids;
    ids.reserve(bnodes.size()); // todo: need to make a keySet() function...
    for(auto const & i : bnodes) {
        ids.push_back(i.first);
    }
    return normalizeUtils.hashBlankNodes(bnodes_insertion_order_keys); // todo: this is the only call to hashblanknodes. that method should just always use all the ids so I don't have to pass them in?
}

