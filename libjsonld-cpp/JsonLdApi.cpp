#include "JsonLdApi.h"
#include "ObjUtils.h"
#include "NormalizeUtils.h"
#include "BlankNode.h"

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

json JsonLdApi::expand(Context activeCtx, std::string * activeProperty, json element, const std::string & baseUrl,
                       bool frameExpansion, bool ordered, bool fromMap) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the Expansion algorithm.
    // See: https://w3c.github.io/json-ld-api/#expansion-algorithm

    // todo: remove
    std::cout << "... 1.1 JsonLdApi::expand()\n";

    // 1)
    if (element.empty()) {
        return element; // todo: was null...
    }

    // 2)
    if (activeProperty != nullptr && *activeProperty == JsonLdConsts::DEFAULT) {
        frameExpansion = false;
    }

    // 3)
    json propertyScopedContext;
    if(activeProperty != nullptr && !activeCtx.getTermDefinition(*activeProperty).is_null() &&
       !activeCtx.getTermDefinition(*activeProperty)[JsonLdConsts::LOCALCONTEXT].is_null()) {
        propertyScopedContext = activeCtx.getTermDefinition(*activeProperty)[JsonLdConsts::LOCALCONTEXT];
    }

    // 5)
    if(element.is_array())
        return expandArrayElement(activeCtx, activeProperty, element, baseUrl, frameExpansion, ordered, fromMap);
    // 6)
    else if(element.is_object())
        return expandObjectElement(activeCtx, activeProperty, element, baseUrl, propertyScopedContext, frameExpansion, ordered, fromMap);
    // 4)
    else // scalar
    {
        // 4.1)
        if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
            return nullptr;
        }
        // 4.2)
        // todo: ...
        // 4.3)
        return activeCtx.expandValue(*activeProperty, element);
    }
}

json JsonLdApi::expandArrayElement(Context activeCtx, std::string * activeProperty, const json& element, const std::string & baseUrl,
                                   bool frameExpansion, bool ordered, bool fromMap) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the Expansion algorithm.
    // See: https://w3c.github.io/json-ld-api/#expansion-algorithm

    // todo: remove
    std::cout << "... 1.1 JsonLdApi::expandArrayElement()\n";

    // 5.1)
    // Initialize an empty array, result.
    json result = json::array();

    // 5.2)
    // For each item in element:
    for (auto &item : element) {
        // 5.2.1)
        // Initialize expanded item to the result of using this algorithm recursively, passing
        // active context, active property, item as element, base URL, the frameExpansion,
        // ordered, and from map flags.
        json expandedItem = expand(activeCtx, activeProperty, item, baseUrl, frameExpansion, ordered, fromMap);

        // 5.2.2)
        // If the container mapping of active property includes @list, and expanded item is
        // an array, set expanded item to a new map containing the entry @list where the value
        // is the original expanded item.
        if(activeProperty != nullptr) {
            auto termDefinition = activeCtx.getTermDefinition(*activeProperty);
            if (!termDefinition.empty()) {
                if (termDefinition.contains(JsonLdConsts::CONTAINER)) {
                    auto containerMapping = termDefinition[JsonLdConsts::CONTAINER];
                    if (arrayContains(containerMapping, JsonLdConsts::LIST) &&
                        expandedItem.is_array())
                        expandedItem = json::object({{JsonLdConsts::LIST, expandedItem}});
                }
            }
        }

        // 5.2.3)
        // If expanded item is an array, append each of its items to result. Otherwise,
        // if expanded item is not null, append it to result.
        if (!expandedItem.is_null()) {
            if (expandedItem.is_array()) {
                result.insert(result.end(), expandedItem.begin(), expandedItem.end());
            } else {
                result.push_back(expandedItem);
            }
        }
    }
    // 5.3)
    return result;
}

json JsonLdApi::expandObjectElement(Context activeCtx, std::string * activeProperty, json element, const std::string & baseUrl,
                                    nlohmann::json propertyScopedContext, bool frameExpansion, bool ordered, bool fromMap) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the Expansion algorithm.
    // See: https://www.w3.org/TR/2014/REC-json-ld-api-20140116/#expansion-algorithm

    // todo: This needs to be upgraded to conform with
    // https://w3c.github.io/json-ld-api/#expansion-algorithm

    // todo: remove
    std::cout << "... 1.1 JsonLdApi::expandObjectElement()\n";

    // 7)
    if(activeCtx.getPreviousContext() != nullptr && !fromMap) {
        throw JsonLdError(JsonLdError::NotImplemented, "need to copy active to previous context...");
    }

    // 8)
    if(!propertyScopedContext.is_null()) {
        throw JsonLdError(JsonLdError::NotImplemented, "need to add overrideProtected flag to parse()");
        // todo: also need to check for term def for activeProperty and then get that term's baseurl
        activeCtx = activeCtx.parse(propertyScopedContext, baseUrl);
    }

    // 9)
    if (element.contains(JsonLdConsts::CONTEXT)) {
        activeCtx = activeCtx.parse(element[JsonLdConsts::CONTEXT], baseUrl);
    }

    // 10)
    Context typeScopedContext = activeCtx;

    // 11), 12)
    std::string inputType = findInputType(activeCtx, typeScopedContext, element); // only used in 13.4.7.1 and 13.4.16

    json result = ObjUtils::newMap();
    json nests = ObjUtils::newMap();

    // 13)
    // For each key and value in element, ordered lexicographically by key if ordered is true:
    std::vector<std::string> element_keys;
    for (json::iterator it = element.begin(); it != element.end(); ++it) {
        element_keys.push_back(it.key());
    }

    if(ordered)
        std::sort(element_keys.begin(), element_keys.end());

    for (auto & key : element_keys) {

        auto element_value = element[key];

        // 13.1)
        // If key is @context, continue to the next key.
        if (key == JsonLdConsts::CONTEXT) {
            continue;
        }

        // 13.2)
        // Initialize expanded property to the result of IRI expanding key.
        std::string expandedProperty = activeCtx.expandIri(key, false, true);
        json expandedValue;

        // 13.3)
        // If expanded property is null or it neither contains a colon (:) nor it is a
        // keyword, drop key by continuing to the next key.
        if (
                expandedProperty.empty() ||
                !(expandedProperty.find(':') != std::string::npos ||
                  JsonLdUtils::isKeyword(expandedProperty))) {
            continue;
        }

        // 13.4)
        // If expanded property is a keyword:
        if (JsonLdUtils::isKeyword(expandedProperty)) {
            // 13.4.1)
            // If active property equals @reverse, an invalid reverse property map error
            // has been detected and processing is aborted.
            if (activeProperty != nullptr && *activeProperty == JsonLdConsts::REVERSE) {
                throw JsonLdError(JsonLdError::InvalidReversePropertyMap,
                                      "a keyword cannot be used as a @reverse property");
            }
            // 13.4.2)
            // If result already has an expanded property entry, other than @included or @type
            // (unless processing mode is json-ld-1.0), a colliding keywords error has been
            // detected and processing is aborted.
            if (result.contains(expandedProperty)) {
                if (activeCtx.isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
                    throw JsonLdError(JsonLdError::CollidingKeywords,
                                      expandedProperty + " already exists in result");
                }
                if(expandedProperty != JsonLdConsts::INCLUDED && expandedProperty != JsonLdConsts::TYPE)
                    throw JsonLdError(JsonLdError::CollidingKeywords,
                                      expandedProperty + " already exists in result");
            }
            // 13.4.3)
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
                            if (!v.is_string()) {
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
            // 13.4.4)
            else if (expandedProperty == JsonLdConsts::TYPE) {
                // 13.4.4.1)
                if (element_value.is_array()) {
                    for ( const auto& v : element_value) {
                        if (!v.is_string()) {
                            throw JsonLdError(JsonLdError::InvalidTypeValue,
                                              "@type value must be a string or array of strings");
                        }
                        expandedValue.push_back(
                                activeCtx.expandIri(v.get<std::string>(), true, true));
                    }
                } else if (element_value.is_string()) {
                    expandedValue = activeCtx.expandIri(element_value.get<std::string>(), true, true);
                }
                else if (options.getFrameExpansion() && element_value.is_object()) {
                    if (!element_value.empty()) {
                        throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a an empty object for framing");
                    }
                } else {
                    throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a string or array of strings");
                }

                // 13.4.4.2)
                if(element_value.is_object())
                    expandedValue = element_value;
                // 13.4.4.3)
                else if(ObjUtils::isDefaultObject(element_value)) {
                    expandedValue = ObjUtils::newMap();
                    expandedValue[JsonLdConsts::DEFAULT] =
                            typeScopedContext.expandIri(element_value.at(JsonLdConsts::DEFAULT), true, true);
                }
                // 13.4.4.4)
                else {

                    if(element_value.is_string()) {
                        expandedValue =
                                typeScopedContext.expandIri(element_value, true, true);
                    }
                    else if(element_value.is_array()) {
                        expandedValue = json::array();
                        for(const auto& v : element_value) {
                            expandedValue.push_back(typeScopedContext.expandIri(v, true, true));
                        }
                    }

                }
                // 13.4.4.5)
                if (result.contains(JsonLdConsts::TYPE)) {

                    json t = json::array();
                    auto typeValue = result.at(JsonLdConsts::TYPE);

                    if (typeValue.is_array()) {
                        t.insert(t.end(), typeValue.begin(), typeValue.end());
                    } else {
                        t.push_back(typeValue);
                    }
                    if (expandedValue.is_array()) {
                        t.insert(t.end(), expandedValue.begin(), expandedValue.end());
                    } else {
                        t.push_back(expandedValue);
                    }
                    expandedValue = t;
                }
            }
            // 13.4.5)
            else if (expandedProperty == JsonLdConsts::GRAPH) {
                std::string currentProperty = JsonLdConsts::GRAPH;
                expandedValue = expand(activeCtx, &currentProperty, element_value, baseUrl, frameExpansion, ordered, fromMap);
                if(!expandedValue.is_array()) {
                    expandedValue = json::array({expandedValue});
                }
            }
            // 13.4.6)
            else if (expandedProperty == JsonLdConsts::INCLUDED) {
                // 13.4.6.1)
                if (activeCtx.isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
                    continue;
                }
                // 13.4.6.2)
                expandedValue = expand(activeCtx, nullptr, element_value, baseUrl, frameExpansion, ordered, fromMap);
                if(!expandedValue.is_array()) {
                    expandedValue = json::array({expandedValue});
                }
                // 13.4.6.3)
                for(const auto& v : expandedValue) {
                    if(!ObjUtils::isNodeObject(v))
                        throw JsonLdError(JsonLdError::InvalidIncludedValue);
                }
                // 13.4.6.4)
                if (result.contains(JsonLdConsts::INCLUDED)) {

                    json t = json::array();
                    auto includedValue = result.at(JsonLdConsts::INCLUDED);

                    if (includedValue.is_array()) {
                        t.insert(t.end(), includedValue.begin(), includedValue.end());
                    } else {
                        t.push_back(includedValue);
                    }
                    if (expandedValue.is_array()) {
                        t.insert(t.end(), expandedValue.begin(), expandedValue.end());
                    } else {
                        t.push_back(expandedValue);
                    }
                    expandedValue = t;
                }
            }
            // 13.4.7)
            // If expanded property is @value
            else if (expandedProperty == JsonLdConsts::VALUE) {
                // 13.4.7.1)
                // If input type is @json, set expanded value to value. If processing mode
                // is json-ld-1.0, an invalid value object value error has been detected and
                // processing is aborted.
                if(inputType == JsonLdConsts::JSON) {
                    if (activeCtx.isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
                        throw JsonLdError(JsonLdError::InvalidValueObjectValue);
                    }
                    expandedValue = element_value;
                }
                // 13.4.7.2)
                // Otherwise, if value is not a scalar or null, an invalid value object
                // value error has been detected and processing is aborted. When the
                // frameExpansion flag is set, value MAY be an empty map or an array
                // of scalar values.
                else if(element_value.is_null() || JsonLdUtils::isScalar(element_value) ||
                        (frameExpansion && (JsonLdUtils::isEmptyObject(element_value) || JsonLdUtils::isArrayOfScalars(element_value)))) {
                    // 13.4.7.3)
                    // Otherwise, set expanded value to value. When the frameExpansion flag
                    // is set, expanded value will be an array of one or more string values
                    // or an array containing an empty map.
                    expandedValue = element_value;
                    if(frameExpansion) {
                        if(!expandedValue.is_array())
                            expandedValue = json::array({expandedValue});
                    }
                }
                else {
                    throw JsonLdError(JsonLdError::InvalidValueObjectValue);
                }
                // 13.4.7.4)
                // If expanded value is null, set the @value entry of result to null and
                // continue with the next key from element. Null values need to be preserved
                // in this case as the meaning of an @type entry depends on the existence of
                // an @value entry.
                if (expandedValue.is_null()) {
                    result[JsonLdConsts::VALUE] = nullptr;
                    continue;
                }
            }
            // 13.4.8)
            else if (expandedProperty == JsonLdConsts::LANGUAGE) {
                // 13.4.8.1)
                if (element_value.is_string() ||
                (frameExpansion && (JsonLdUtils::isEmptyObject(element_value) ||
                JsonLdUtils::isEmptyArray(element_value) ||
                JsonLdUtils::isArrayOfStrings(element_value)))) {
                    // 13.4.8.2)
                    std::string v = element_value.get<std::string>();
                    // todo: add warning about non-well-formed language strings
                    std::transform(v.begin(), v.end(), v.begin(), &::tolower);
                    expandedValue = v;
                } else {
                    throw JsonLdError(JsonLdError::InvalidLanguageTaggedString);
                }
            }
            // 13.4.9)
            else if (expandedProperty == JsonLdConsts::DIRECTION) {
                // 13.4.9.1)
                if (activeCtx.isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
                    continue;
                }
                // 13.4.9.2)
                if ((element_value.is_string() && (element_value == "ltr" || element_value == "rtl")) ||
                    (frameExpansion && (JsonLdUtils::isEmptyObject(element_value) ||
                                        JsonLdUtils::isEmptyArray(element_value) ||
                                        JsonLdUtils::isArrayOfStrings(element_value)))) {
                    // 13.4.9.3)
                    expandedValue = element_value;
                    if(frameExpansion) {
                        if(!expandedValue.is_array()) {
                            expandedValue = json::array({expandedValue});
                        }
                    }
                } else {
                    throw JsonLdError(JsonLdError::InvalidLanguageTaggedString);
                }
            }
            // 13.4.10)
            else if (expandedProperty == JsonLdConsts::INDEX) {
                if (!element_value.is_string()) {
                    throw JsonLdError(JsonLdError::InvalidIndexValue,
                                          "Value of " + expandedProperty + " must be a string");
                }
                expandedValue = element_value;
            }
            // 13.4.11)
            else if (expandedProperty == JsonLdConsts::LIST) {
                // 13.4.11.1)
                if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
                    continue;
                }
                // 13.4.11.2)
                expandedValue = expand(activeCtx, activeProperty, element_value, baseUrl, frameExpansion, ordered);
                if (!expandedValue.is_array()) {
                    expandedValue = json::array({expandedValue});
                }
            }
            // 13.4.12)
            else if (expandedProperty == JsonLdConsts::SET) {
                expandedValue = expand(activeCtx, activeProperty, element_value, baseUrl, frameExpansion, ordered);
            }
            // 13.4.13)
            else if (expandedProperty == JsonLdConsts::REVERSE) {
                // 13.4.13.1)
                if (!element_value.is_object()) {
                    throw JsonLdError(JsonLdError::InvalidReverseValue,
                                          "@reverse value must be an object");
                }
                // 13.4.13.2)
                {
                    std::string currentProperty = JsonLdConsts::REVERSE;
                    expandedValue = expand(activeCtx, &currentProperty, element_value, baseUrl, frameExpansion, ordered);
                }
                // 13.4.13.3)
                if (expandedValue.contains(JsonLdConsts::REVERSE)) {
                    // 13.4.13.3.1)
                    for (const auto& entry : expandedValue[JsonLdConsts::REVERSE].items()) {
                        JsonLdUtils::addValue(result, entry.key(), entry.value(), true);
                    }
                }
                // 13.4.13.4)
                if (expandedValue.size() > (expandedValue.contains(JsonLdConsts::REVERSE) ? 1 : 0)) {
                    // 13.4.13.4.1)
                    if (!result.contains(JsonLdConsts::REVERSE)) {
                        result[JsonLdConsts::REVERSE] = json::object();
                    }
                    auto & reverseMap = result[JsonLdConsts::REVERSE];

                    // 13.4.13.4.2)
                    for (const auto& entry : expandedValue.items())
                    {
                        if(entry.key() == JsonLdConsts::REVERSE)
                            continue;

                        // 13.4.13.4.2.1)
                        for(const auto& item : entry.value()) {
                            // 13.4.13.4.2.1.1)
                            // If item is a value object or list object, an invalid reverse property
                            // value has been detected and processing is aborted.
                            if(JsonLdUtils::isValueObject(item) || JsonLdUtils::isListObject(item))
                                throw JsonLdError(JsonLdError::InvalidReversePropertyValue);

                            // 13.4.13.4.2.1.2)
                            // Use add value to add item to the property entry in reverse map using
                            // true for as array.
                            JsonLdUtils::addValue(reverseMap, entry.key(), item, true);
                        }
                    }
                }
                // 13.4.13.5)
                continue;
            }

            // 13.4.14)
            if (expandedProperty == JsonLdConsts::NEST) {
                if (!nests.contains(key)) {
                    nests[key] = json::array();
                }
                continue;
            }

            // 13.4.15)
            if (frameExpansion &&
                (expandedProperty == JsonLdConsts::DEFAULT ||
                expandedProperty == JsonLdConsts::EMBED ||
                expandedProperty == JsonLdConsts::EXPLICIT ||
                expandedProperty == JsonLdConsts::REQUIRE_ALL ||
                expandedProperty == JsonLdConsts::OMIT_DEFAULT)) {
                expandedValue = expand(activeCtx, &expandedProperty, element_value, baseUrl, frameExpansion, ordered);
            }

            // 13.4.16)
            // Unless expanded value is null, expanded property is @value, and input type is
            // not @json, set the expanded property entry of result to expanded value.
            if (!(expandedValue.is_null()) ||
                (expandedProperty == JsonLdConsts::VALUE && inputType != JsonLdConsts::JSON)) {
                // note: setting null instead of empty string, since expandIRI might return that
                if(expandedValue.is_string() && expandedValue.get<std::string>().empty())
                    result[expandedProperty] = nullptr;
                else
                    result[expandedProperty] = expandedValue;
            }

            // 13.4.17
            continue;
        }

        // 13.5)
        auto keyTermDefinition = activeCtx.getTermDefinition(key);

        json containerMapping;
        if(keyTermDefinition.contains(JsonLdConsts::CONTAINER)) {
            containerMapping = keyTermDefinition[JsonLdConsts::CONTAINER];
        }

        // 13.6)
        if(keyTermDefinition.contains(JsonLdConsts::TYPE) &&
           keyTermDefinition.at(JsonLdConsts::TYPE) == JsonLdConsts::JSON) {
            expandedValue = ObjUtils::newMap();
            expandedValue[JsonLdConsts::VALUE] = element_value;
            expandedValue[JsonLdConsts::TYPE] = JsonLdConsts::JSON;
        }
        // 13.7)
        // Otherwise, if container mapping includes @language and value is a map then
        // value is expanded from a language map as follows:
        else if (arrayContains(containerMapping, JsonLdConsts::LANGUAGE) && element_value.is_object()) {
            // 13.7.1)
            // Initialize expanded value to an empty array.
            expandedValue = json::array();
            // 13.7.2)
            // Initialize direction to the default base direction from active context.
            json direction = activeCtx.getDefaultBaseDirection();
            // 13.7.3)
            // If key's term definition in active context has a direction mapping, update
            // direction with that value.
            if(keyTermDefinition.contains(JsonLdConsts::DIRECTION))
                direction = keyTermDefinition.at(JsonLdConsts::DIRECTION);

            // 13.7.4)
            // For each key-value pair language-language value in value, ordered
            // lexicographically by language if ordered is true:
            std::vector<std::string> value_keys;
            for (json::iterator it = element_value.begin(); it != element_value.end(); ++it) {
                value_keys.push_back(it.key());
            }
            if(ordered)
                std::sort(value_keys.begin(), value_keys.end());

            for(auto& language : value_keys) {
                json languageValue = element_value[language];

                // 13.7.4.1)
                // If language value is not an array set language value to an array
                // containing only language value.
                if (!languageValue.is_array()) {
                    languageValue = json::array({languageValue});
                }

                // 13.7.4.2)
                // For each item in language value:
                for ( const auto& item : languageValue) {
                    // 13.7.4.2.1)
                    // If item is null, continue to the next entry in language value.
                    if(item.is_null())
                        continue;
                    // 13.7.4.2.2)
                    // item must be a string, otherwise an invalid language map value error
                    // has been detected and processing is aborted.
                    if (!item.is_string()) {
                        std::stringstream ss;
                        ss << "Expected: " << item << " to be a string";
                        throw JsonLdError(JsonLdError::InvalidLanguageMapValue, ss.str());
                    }
                    // 13.7.4.2.3)
                    // Initialize a new map v consisting of two key-value pairs: (@value-item)
                    // and (@language-language). If item is neither @none nor well-formed
                    // according to section 2.2.9 of [BCP47], processors SHOULD issue a
                    // warning. Note: Processors MAY normalize language tags to lower case.
                    json v = ObjUtils::newMap();
                    std::transform(language.begin(), language.end(), language.begin(), &::tolower);
                    v[JsonLdConsts::VALUE] = item;
                    v[JsonLdConsts::LANGUAGE] = language;
                    // todo: issue warnings

                    // 13.7.4.2.4)
                    // If language is @none, or expands to @none, remove @language from v.
                    if(language != JsonLdConsts::NONE) {
                        auto expandedLanguage  = activeCtx.expandIri(language, false, true);
                        if(expandedLanguage != JsonLdConsts::NONE) {
                            v[JsonLdConsts::LANGUAGE] = language;
                        }
                    }

                    // 13.7.4.2.5)
                    // If direction is not null, add an entry for @direction to v with direction.
                    if(!direction.is_null() &&
                       (direction.is_string() && !direction.get<std::string>().empty() && direction != "null"))
                        v[JsonLdConsts::DIRECTION] = direction;

                    // 13.7.4.2.6)
                    // Append v to expanded value.
                    expandedValue.push_back(v);
                }
            }
        }
        // 13.8)
        // Otherwise, if container mapping includes @index, @type, or @id and value is a
        // map then value is expanded from an map as follows:
        else if (element_value.is_object() &&
            (arrayContains(containerMapping, JsonLdConsts::INDEX) ||
             arrayContains(containerMapping, JsonLdConsts::TYPE) ||
             arrayContains(containerMapping, JsonLdConsts::ID))) {

            // 13.8.1)
            // Initialize expanded value to an empty array.
            expandedValue = json::array();

            // 13.8.2)
            // Initialize index key to the key's index mapping in active context, or
            // @index, if it does not exist.
            std::string indexKey = JsonLdConsts::INDEX;
            if(keyTermDefinition.contains(JsonLdConsts::INDEX))
                indexKey = keyTermDefinition.at(JsonLdConsts::INDEX).get<std::string>();

            // 13.8.3)
            // For each key-value pair index-index value in value, ordered lexicographically
            // by index if ordered is true:
            std::vector<std::string> indexKeys;
            for(auto& el : element_value.items()) {
                indexKeys.push_back(el.key());
            }
            if(ordered)
                std::sort(indexKeys.begin(), indexKeys.end());

            for ( const auto& index : indexKeys) {
                json indexValue = element_value[index];

                // 13.8.3.1)
                // If container mapping includes @id or @type, initialize map context to the previous context
                // from active context if it exists, otherwise, set map context to active context.
                Context mapContext = activeCtx;
                if(arrayContains(containerMapping, JsonLdConsts::TYPE) ||
                   arrayContains(containerMapping, JsonLdConsts::ID)) {
                    if(activeCtx.getPreviousContext())
                        mapContext = *activeCtx.getPreviousContext();
                }

                // 13.8.3.2)
                // If container mapping includes @type and index's term definition in map
                // context has a local context, update map context to the result of the
                // Context Processing algorithm, passing map context as active context the
                // value of the index's local context as local context and base URL from
                // the term definition for index in map context.
                if(arrayContains(containerMapping, JsonLdConsts::TYPE)) {
                    auto indexTermDefinition = mapContext.getTermDefinition(index);
                    if(indexTermDefinition.contains(JsonLdConsts::LOCALCONTEXT)) {
                        mapContext = mapContext.parse(
                                indexTermDefinition.at(JsonLdConsts::LOCALCONTEXT),
                                indexTermDefinition.at(JsonLdConsts::BASEURL));
                    }
                }
                // 13.8.3.3)
                // Otherwise, set map context to active context.
                else
                    mapContext = activeCtx;

                // 13.8.3.4)
                // Initialize expanded index to the result of IRI expanding index.
                auto expandedIndex = activeCtx.expandIri(index, false, true);

                // 13.8.3.5)
                // If index value is not an array set index value to an array containing only
                // index value.
                if (!indexValue.is_array()) {
                    indexValue = json::array({indexValue});
                }

                // 13.8.3.6)
                // Initialize index value to the result of using this algorithm recursively,
                // passing map context as active context, key as active property, index value
                // as element, base URL, true for from map, and the frameExpansion and ordered flags.
                indexValue = expand(activeCtx, &key, indexValue, baseUrl, frameExpansion, ordered, true);

                // 13.8.3.7)
                // For each item in index value:
                for ( auto item : indexValue) {

                    // 13.8.3.7.1)
                    // If container mapping includes @graph, and item is not a graph object, set
                    // item to a new map containing the key-value pair @graph-item, ensuring that
                    // the value is represented using an array.
                    if(arrayContains(containerMapping, JsonLdConsts::GRAPH) &&
                            !JsonLdUtils::isGraphObject(item)) {
                        if(!item.is_array())
                            item = json::array({item});
                        item = json::object({{JsonLdConsts::GRAPH, item}});
                    }

                    // 13.8.3.7.2)
                    // If container mapping includes @index, index key is not @index, and expanded
                    // index is not @none:
                    if(arrayContains(containerMapping, JsonLdConsts::INDEX) &&
                       indexKey != JsonLdConsts::INDEX &&
                       expandedIndex != JsonLdConsts::NONE) {

                        // 13.8.3.7.2.1)
                        // Initialize re-expanded index to the result of calling the Value Expansion
                        // algorithm, passing the active context, index key as active property, and
                        // index as value.
                        auto reExpandedIndex = activeCtx.expandValue(indexKey, index);

                        // 13.8.3.7.2.2)
                        // Initialize expanded index key to the result of IRI expanding index key.
                        auto expandedIndexKey = activeCtx.expandIri(indexKey, false, true);

                        // 13.8.3.7.2.3)
                        // Initialize index property values to an array consisting of re-expanded
                        // index followed by the existing values of the concatenation of expanded
                        // index key in item, if any.
                        json indexPropertyValues = json::array({reExpandedIndex});
                        for(const auto& el : item[expandedIndexKey])
                            indexPropertyValues.push_back(el);

                        // 13.8.3.7.2.4)
                        // Add the key-value pair (expanded index key-index property values) to item.
                        item[expandedIndexKey] = indexPropertyValues;

                        // 13.8.3.7.2.5)
                        // If item is a value object, it MUST NOT contain any extra properties; an
                        // invalid value object error has been detected and processing is aborted.
                        if(JsonLdUtils::isValueObject(item) && item.size() > 1)
                            throw JsonLdError(JsonLdError::InvalidValueObject);
                    }

                    // 13.8.3.7.3)
                    // Otherwise, if container mapping includes @index, item does not have an
                    // entry @index, and expanded index is not @none, add the key-value pair
                    // (@index-index) to item.
                    else if(arrayContains(containerMapping, JsonLdConsts::INDEX) &&
                            !item.contains(JsonLdConsts::INDEX) &&
                            expandedIndex != JsonLdConsts::NONE) {
                        item[JsonLdConsts::INDEX] = index;
                    }

                    // 13.8.3.7.4)
                    // Otherwise, if container mapping includes @id, item does not have the entry
                    // @id, and expanded index is not @none, add the key-value pair
                    // (@id-expanded index) to item, where expanded index is set to the result
                    // of IRI expandingindex using true for document relative and false for vocab.
                    else if(arrayContains(containerMapping, JsonLdConsts::ID) &&
                            !item.contains(JsonLdConsts::ID) &&
                            expandedIndex != JsonLdConsts::NONE) {
                        auto expandedIndex2 = activeCtx.expandIri(index, true, false);
                        item[JsonLdConsts::ID] = expandedIndex2;
                    }

                    // 13.8.3.7.5)
                    // Otherwise, if container mapping includes @type and expanded index is
                    // not @none, initialize types to a new array consisting of expanded index
                    // followed by any existing values of @type in item. Add the key-value pair
                    // (@type-types) to item.
                    else if(arrayContains(containerMapping, JsonLdConsts::TYPE) &&
                            expandedIndex != JsonLdConsts::NONE) {
                        auto itemTypes = item[JsonLdConsts::TYPE];
                        json types = json::array({expandedIndex});
                        if(!itemTypes.is_null()) {
                            if(itemTypes.is_array())
                                types.insert(types.end(), itemTypes.begin(), itemTypes.end());
                            else
                                types.push_back(itemTypes);
                        }
                        item[JsonLdConsts::TYPE] = types;
                    }

                    // 13.8.3.7.6)
                    // Append item to expanded value.
                    expandedValue.push_back(item);
                }
            }
        }
        // 13.9)
        // Otherwise, initialize expanded value to the result of using this algorithm
        // recursively, passing active context, key for active property, value for element,
        // base URL, and the frameExpansion and ordered flags.
        else {
            expandedValue = expand(activeCtx, &key, element_value, baseUrl, frameExpansion, ordered, false);
        }

        // 13.10)
        // If expanded value is null, ignore key by continuing to the next key from element.
        if (expandedValue.is_null()) {
            continue;
        }

        // 13.11)
        // If container mapping includes @list and expanded value is not already a list object,
        // convert expanded value to a list object by first setting it to an array containing only
        // expanded value if it is not already an array, and then by setting it to a map
        // containing the key-value pair @list-expanded value.
        if (arrayContains(containerMapping, JsonLdConsts::LIST) &&
            !JsonLdUtils::isListObject(expandedValue)) {
            if(!expandedValue.is_array())
                expandedValue = json::array({expandedValue});
            expandedValue = json::object({{JsonLdConsts::LIST, expandedValue}});
        }

        // 13.12)
        // If container mapping includes @graph, and includes neither @id nor @index, convert
        // expanded value into an array, if necessary, then convert each value ev in expanded
        // value into a graph object:
        if (arrayContains(containerMapping, JsonLdConsts::GRAPH) &&
            !arrayContains(containerMapping, JsonLdConsts::ID) &&
            !arrayContains(containerMapping, JsonLdConsts::INDEX)) {

            if(!expandedValue.is_array())
                expandedValue = json::array({expandedValue});

            json t = json::array();

            // 13.12.1)
            // Convert ev into a graph object by creating a map containing the key-value
            // pair @graph-ev where ev is represented as an array.
            for(const auto& ev : expandedValue) {
                if(!ev.is_array())
                    t.push_back(json::object({{JsonLdConsts::GRAPH, json::array({ev})}}));
                else
                    t.push_back(json::object({{JsonLdConsts::GRAPH, ev}}));
            }

            expandedValue = t;
        }



        // 13.13)
        // If the term definition associated to key indicates that it is a reverse property
        if (activeCtx.isReverseProperty(key)) {
            // 13.13.1)
            // If result has no @reverse entry, create one and initialize its value to an empty map.
            if (!result.contains(JsonLdConsts::REVERSE)) {
                result[JsonLdConsts::REVERSE] = ObjUtils::newMap();
            }

            // 13.13.2)
            // Reference the value of the @reverse entry in result using the variable reverse map.
             auto & reverseMap = result[JsonLdConsts::REVERSE];

            // 13.13.3)
            // If expanded value is not an array, set it to an array containing expanded value.
            if (!expandedValue.is_array())
                expandedValue = json::array({expandedValue});

            // 13.13.4)
            // For each item in expanded value
            for ( auto item : expandedValue) {
                // 13.13.4.1)
                // If item is a value object or list object, an invalid reverse property value
                // has been detected and processing is aborted.
                if (JsonLdUtils::isValueObject(item) || JsonLdUtils::isListObject(item)) {
                    throw JsonLdError(JsonLdError::InvalidReversePropertyValue);
                }
                // 13.13.4.2)
                // If reverse map has no expanded property entry, create one and initialize
                // its value to an empty array.
                if (!reverseMap.contains(expandedProperty)) {
                    reverseMap[expandedProperty] = json::array();
                }
                // 13.13.4.3)
                // Use add value to add item to the expanded property entry in reverse map
                // using true for as array.
                JsonLdUtils::addValue(reverseMap, expandedProperty, item, true);
            }
        }
        // 13.14)
        // Otherwise, key is not a reverse property use add value to add expanded value to
        // the expanded property entry in result using true for as array.
        else {
            JsonLdUtils::addValue(result, expandedProperty, expandedValue, true);
        }
    }

    // 14)
    // For each key nesting-key in nests, ordered lexicographically if ordered is true:
    std::vector<std::string> nestKeys;
    for(auto& el : nests.items()) {
        nestKeys.push_back(el.key());
    }
    if(ordered)
        std::sort(nestKeys.begin(), nestKeys.end());

    for ( const auto& nestingKey : nestKeys) {

        // 14.1)
        // Initialize nested values to the value of nesting-key in element, ensuring
        // that it is an array.
        auto nestedValues = element[nestingKey];
        if(!nestedValues.is_array())
            nestedValues = json::array({nestedValues});

        // 14.2)
        // For each nested value in nested values:
        for(auto nestedValue : nestedValues) {

            // 14.2.1)
            // If nested value is not a map, or any key within nested value expands to
            // @value, an invalid @nest value error has been detected and processing is aborted.
            if(!nestedValue.is_object()) {
                throw JsonLdError(JsonLdError::InvalidNestValue);
            }

            // 14.2.2)
            // Recursively repeat steps 3, 8, 13, and 14 using nesting-key for active
            // property, and nested value for element.

            // todo: ...
            throw JsonLdError(JsonLdError::NotImplemented, "need to add implement @nest handling");

        }
    }

    // 15)
    // If result contains the entry @value:
    if (result.contains(JsonLdConsts::VALUE)) {

        // 15.1)
        // The result must not contain any entries other than @direction, @index, @language,
        // @type, and @value. It must not contain an @type entry if it contains either
        // @language or @direction entries. Otherwise, an invalid value object error has
        // been detected and processing is aborted.
        std::set<std::string> validKeywords {
                JsonLdConsts::DIRECTION,JsonLdConsts::INDEX,JsonLdConsts::LANGUAGE,
                JsonLdConsts::TYPE,JsonLdConsts::VALUE
        };
        for (auto& el : result.items()) {
            if(validKeywords.find(el.key()) == validKeywords.end())
                throw JsonLdError(JsonLdError::InvalidValueObject,el.key() + " not in list of valid keywords");
        }
        if(result.contains(JsonLdConsts::TYPE) &&
           (result.contains(JsonLdConsts::LANGUAGE) || result.contains(JsonLdConsts::DIRECTION))) {
            throw JsonLdError(JsonLdError::InvalidValueObject);
        }

        // 15.2)
        // If the result's @type entry is @json, then the @value entry may contain any
        // value, and is treated as a JSON literal.
        if(!result.contains(JsonLdConsts::TYPE) ||
           (!JsonLdUtils::containsOrEquals(result[JsonLdConsts::TYPE], JsonLdConsts::JSON))) {
            // 15.3)
            // Otherwise, if the value of result's @value entry is null, or an empty
            // array, return null.
            if(result[JsonLdConsts::VALUE].is_null() || (result[JsonLdConsts::VALUE].is_array() && result[JsonLdConsts::VALUE].empty()))
                return json();

            // 15.4)
            // Otherwise, if the value of result's @value entry is not a string and result
            // contains the entry @language, an invalid language-tagged value error has been
            // detected (only strings can be language-tagged) and processing is aborted.
            else if(!result[JsonLdConsts::VALUE].is_string() && result.contains(JsonLdConsts::LANGUAGE))
                throw JsonLdError(JsonLdError::InvalidLanguageTaggedValue);

            // 15.5)
            // Otherwise, if the result has an @type entry and its value is not an IRI, an
            // invalid typed value error has been detected and processing is aborted.
            else if (result.contains(JsonLdConsts::TYPE) &&
                     (!result[JsonLdConsts::TYPE].is_string() || !JsonLdUtils::isAbsoluteIri(result[JsonLdConsts::TYPE])))
                throw JsonLdError(JsonLdError::InvalidTypedValue);
        }
    }

    // 16)
    // Otherwise, if result contains the entry @type and its associated value is not an
    // array, set it to an array containing only the associated value.
    else if (result.contains(JsonLdConsts::TYPE)) {
        if (!result[JsonLdConsts::TYPE].is_array()) {
            result[JsonLdConsts::TYPE] = json::array({result[JsonLdConsts::TYPE]});
        }
    }

    // 17)
    // Otherwise, if result contains the entry @set or @list:
    else if (result.contains(JsonLdConsts::SET) || result.contains(JsonLdConsts::LIST)) {
        // 17.1)
        // The result must contain at most one other entry which must be @index. Otherwise, an
        // invalid set or list object error has been detected and processing is aborted.
        if (result.size() > (result.contains(JsonLdConsts::INDEX) ? 2 : 1))
            throw JsonLdError(JsonLdError::InvalidSetOrListObject);

        // 17.2)
        // If result contains the entry @set, then set result to the entry's associated value.
        if (result.contains(JsonLdConsts::SET)) {
            result = result[JsonLdConsts::SET];
        }
    }

    // 18)
    // If result is a map that contains only the entry @language, return null.
    if (result.is_object() && result.contains(JsonLdConsts::LANGUAGE) && result.size() == 1) {
        return json();
    }

    // 19)
    // If active property is null or @graph, drop free-floating values as follows:
    if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
        // 19.1)
        // If result is a map which is empty, or contains only the entries @value or @list, set result to null.
        json j;
        if (!result.is_null() && result.is_object() &&
            (result.empty() || result.contains(JsonLdConsts::VALUE) || result.contains(JsonLdConsts::LIST))) {
            result = j;
        }
        // 19.2)
        // Otherwise, if result is a map whose only entry is @id, set result to null. When the
        // frameExpansion flag is set, a map containing only the @id entry is retained.
        else if (!result.is_null() && !options.getFrameExpansion() && result.contains(JsonLdConsts::ID)
                 && result.size() == 1) {
            result = j;
        }
    }

    // 20)
    return result;
}

bool JsonLdApi::arrayContains(const json &array,
                              const std::string &value) const {
    return array.is_array() && std::find(array.cbegin(), array.cend(), value) != array.end();
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

        RDF::RDFGraphName gname =
                RDF::RDFGraphName::createRDFGraphName(graphName, RDF::RDFGraphName::DEFAULT);

        if(graphName != JsonLdConsts::DEFAULT) {
            if (BlankNode::isBlankNodeName(graphName)) {
                gname = RDF::RDFGraphName::createRDFGraphName(graphName, RDF::RDFGraphName::BLANKNODE);
            } else if (JsonLdUtils::isAbsoluteIri(graphName)) {
                gname = RDF::RDFGraphName::createRDFGraphName(graphName, RDF::RDFGraphName::IRI);
            } else {
                continue;
            }
        }

        json & graph = nodeMap[graphName];
        dataset.graphToRDF(graphName, graph);
        // todo: dataset.graphToRDF(gname, graph);

    }

    return dataset;
}

void JsonLdApi::generateNodeMap(json & element, json &nodeMap, std::string *activeGraph, nlohmann::json *activeSubject,
                                std::string *activeProperty, json *list)
{
    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the node map generation algorithm.
    // See: https://www.w3.org/TR/2014/REC-json-ld-api-20140116/#node-map-generation

    // todo: This needs to be upgraded to conform with
    // https://w3c.github.io/json-ld-api/#node-map-generation


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
    return normalizeUtils.hashBlankNodes(bnodes_insertion_order_keys);
}

std::string JsonLdApi::findInputType(Context &activeContext, Context &typeScopedContext, nlohmann::json &element) {

    std::string inputType;
    std::string typeKey;

    // 11)
    std::vector<std::string> element_keys;
    for (json::iterator it = element.begin(); it != element.end(); ++it) {
        element_keys.push_back(it.key());
    }
    std::sort(element_keys.begin(), element_keys.end());
    for (auto & key : element_keys) {

        std::string expandedKey = activeContext.expandIri(key, false, true);

        if (expandedKey != JsonLdConsts::TYPE)
            continue;
        else if (typeKey.empty())
            typeKey = key;

        // 11.1)
        auto element_value = element[key];

        if (!element_value.is_array())
            element_value = json::array({element_value});

        // 11.2)
        std::vector<std::string> terms;
        for (const auto &term : element_value) {
            if (term.is_string())
                terms.push_back(term.get<std::string>());
        }

        std::sort(terms.begin(), terms.end());

        for (auto &term : terms) {
            if (typeScopedContext.getTermDefinition(term).contains(JsonLdConsts::LOCALCONTEXT)) {
                auto termValue = activeContext.getTermDefinition(term);
                auto localContext = typeScopedContext.getTermDefinition(term).at(JsonLdConsts::LOCALCONTEXT);
                std::vector<std::string> remoteContexts;
                activeContext = activeContext.parse(localContext, termValue.at(JsonLdConsts::BASEURL),
                                                    remoteContexts, false, false, true);
            }
        }
    }

    // 12)
    if(!typeKey.empty()) {

        auto t = element.at(typeKey);
        std::string lastValue;

        if(t.is_string())
            lastValue = t.get<std::string>();
        else if(t.is_array()) {
            std::vector<std::string> values;
            for(const auto& v : t) {
                if(v.is_string())
                    values.push_back(v.get<std::string>());
            }
            if(!values.empty()) {
                std::sort(values.begin(), values.end());
                lastValue = values.back();
            }
        }

        if(!lastValue.empty()) {
            inputType = activeContext.expandIri(lastValue,false,true);
        }
    }

    return inputType;
}


