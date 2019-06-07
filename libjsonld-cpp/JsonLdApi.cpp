#include "JsonLdApi.h"
#include "ObjUtils.h"

#include <utility>
#include <iostream>
#include <set>

json JsonLdApi::expand(Context activeCtx, json element) {
    return expand(std::move(activeCtx), nullptr, std::move(element));
}

json JsonLdApi::expand(Context activeCtx, std::string * activeProperty, json element) {

    std::cout << "JsonLdApi::expand: " << element << std::endl;

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

json JsonLdApi::expandArrayElement(Context activeCtx, std::string * activeProperty, json element) {
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
        std::cout << "after parsing1:" << std::endl;
        activeCtx.printInternalMap();
    }
    // 6)
    json result = ObjUtils::newMap();
    // 7)
    std::vector<std::string> keys;
    for (json::iterator it = element.begin(); it != element.end(); ++it) {
        keys.push_back(it.key());
    }
    std::sort(keys.begin(), keys.end());
    for (auto & key : keys) {
        auto value = element[key];
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
                expandedProperty == "" ||
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
                if (value.is_string()) {
                    expandedValue = activeCtx.expandIri(value.get<std::string>(), true, false);
                } else if (options.getFrameExpansion()) {
                    if (value.is_object()) {
                        if (value.size() != 0) {
                            throw JsonLdError(JsonLdError::InvalidIdValue,
                                              "@id value must be a an empty object for framing");
                        }
                        expandedValue = value;
                    } else if (value.is_array()) {
                        //expandedValue = new std::vector<std::std::string>();
                        for ( auto v : value) {
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
                if (value.is_array()) {
                    //expandedValue = new std::vector<std::string>();
                    for ( auto v : value) {
                        if (!(v.is_string())) {
                            throw JsonLdError(JsonLdError::InvalidTypeValue,
                                              "@type value must be a string or array of strings");
                        }
                        expandedValue.push_back(
                                activeCtx.expandIri(v.get<std::string>(), true, true));
                    }
                } else if (value.is_string()) {
                    expandedValue = activeCtx.expandIri(value.get<std::string>(), true, true);
                }
                // TODO: SPEC: no mention of empty map check
                else if (options.getFrameExpansion() && value.is_object()) {
                    if (value.size() != 0) {
                        throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a an empty object for framing");
                    }
                    expandedValue = value;
                } else {
                    throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a string or array of strings");
                }
            }
                // 7.4.5)
            else if (expandedProperty == JsonLdConsts::GRAPH) {
                std::string currentProperty = JsonLdConsts::GRAPH;
                expandedValue = expand(activeCtx, &currentProperty, value);
            }
                // 7.4.6)
            else if (expandedProperty == JsonLdConsts::VALUE) {
                if (!value.is_null() && (value.is_object() || value.is_array())) {
                    throw JsonLdError(JsonLdError::InvalidValueObjectValue,
                                          "value of " + expandedProperty + " must be a scalar or null");
                }
                expandedValue = value;
                if (expandedValue.is_null()) {
                    result[JsonLdConsts::VALUE] = expandedValue; // was null
                    continue;
                }
            }
                // 7.4.7)
            else if (expandedProperty == JsonLdConsts::LANGUAGE) {
                if (!(value.is_string())) {
                    throw JsonLdError(JsonLdError::InvalidLanguageTaggedString,
                                          "Value of " + expandedProperty + " must be a string");
                }
                std::string v = value.get<std::string>();
                std::transform(v.begin(), v.end(), v.begin(), &::tolower);
                expandedValue = v;
            }
                // 7.4.8)
            else if (expandedProperty == JsonLdConsts::INDEX) {
                if (!(value.is_string())) {
                    throw JsonLdError(JsonLdError::InvalidIndexValue,
                                          "Value of " + expandedProperty + " must be a string");
                }
                expandedValue = value;
            }
                // 7.4.9)
            else if (expandedProperty == JsonLdConsts::LIST) {
                // 7.4.9.1)
                if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
                    continue;
                }
                // 7.4.9.2)
                expandedValue = expand(activeCtx, activeProperty, value);

                // NOTE: step not in the spec yet
                if (!(expandedValue.is_array())) {
                    json j;
                    j.push_back(expandedValue);
                    expandedValue = j;
                }

                // 7.4.9.3)
                for ( auto v : expandedValue) {
                    if (v.is_object() && v.contains(JsonLdConsts::LIST)) {
                        throw JsonLdError(JsonLdError::ListOfLists,
                                              "A list may not contain another list");
                    }
                }
            }
                // 7.4.10)
            else if (expandedProperty == JsonLdConsts::SET) {
                expandedValue = expand(activeCtx, activeProperty, value);
            }
                // 7.4.11)
            else if (expandedProperty == JsonLdConsts::REVERSE) {
                if (!(value.is_object())) {
                    throw JsonLdError(JsonLdError::InvalidReverseValue,
                                          "@reverse value must be an object");
                }
                // 7.4.11.1)
                {
                    std::string currentProperty = JsonLdConsts::REVERSE;
                    expandedValue = expand(activeCtx, &currentProperty, value);
                }
                // NOTE: algorithm assumes the result is a map
                // 7.4.11.2)
                if (expandedValue.contains(JsonLdConsts::REVERSE)) {
                    auto reverse = expandedValue[JsonLdConsts::REVERSE];
                    std::vector<std::string> keys;
                    for (json::iterator it = reverse.begin(); it != reverse.end(); ++it) {
                        keys.push_back(it.key());
                    }
                    for ( auto property : keys) {
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
                if (expandedValue.size() > expandedValue.contains(JsonLdConsts::REVERSE) ? 1 : 0) {
                    // 7.4.11.3.1)
                    if (!result.contains(JsonLdConsts::REVERSE)) {
                        result[JsonLdConsts::REVERSE] = json::object();
                    }
                    // 7.4.11.3.2)
                    auto & reverseMap = result[JsonLdConsts::REVERSE];
                    // 7.4.11.3.3)
                    std::vector<std::string> keys;
                    for (json::iterator it = expandedValue.begin(); it != expandedValue.end(); ++it) {
                        keys.push_back(it.key());
                    }
                    for ( auto property : keys) {
                        if (property == JsonLdConsts::REVERSE) {
                            continue;
                        }
                        // 7.4.11.3.3.1)
                        auto items = expandedValue[property];
                        for ( auto item : items) {
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
                expandedValue = expand(activeCtx, &expandedProperty, value);
            }
            // 7.4.12)
            if (!expandedValue.is_null()) {
                result[expandedProperty] = expandedValue;
            }
            // 7.4.13)
            continue;
        }
            // 7.5
        else if (activeCtx.getContainer(key) == JsonLdConsts::LANGUAGE && value.is_object()) {
            // 7.5.1)
//            expandedValue = new ArrayList<Object>();
            // 7.5.2)
//             for ( std::string language : ((Map<std::string, Object>) value).keySet()) {
//                 Object languageValue = ((Map<std::string, Object>) value).get(language);
            for(auto& el : value.items()) {
                std::string language = el.key();
                json languageValue = el.value();

                // 7.5.2.1)
                if (!(languageValue.is_array())) {
                    json j;
                    j.push_back(languageValue);
                    languageValue = j;
                }
                // 7.5.2.2)
                for ( auto item : languageValue) {
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
                 && value.is_object()) {
            // 7.6.1)
//            expandedValue = new ArrayList<Object>();
            // 7.6.2)
            std::vector<std::string> indexKeys;
            for(auto& el : value.items()) {
                indexKeys.push_back(el.key());
            }
            std::sort(indexKeys.begin(), indexKeys.end());
            for ( auto index : indexKeys) {
                json indexValue = value[index];
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
            expandedValue = expand(activeCtx, &key, value);
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
            if( jStr.find("_:") == 0 || jStr.find(":") == std::string::npos) {
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

