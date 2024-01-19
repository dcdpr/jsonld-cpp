#include "jsonld-cpp/ExpansionProcessor.h"
#include "jsonld-cpp/Context.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/ContextProcessor.h"
#include "WellFormed.h"
#include <set>
#include <string>
#include <iostream>

using nlohmann::json;

namespace {

    bool arrayContains(const json &array, const std::string &value) {
        return array.is_array() && std::find(array.cbegin(), array.cend(), value) != array.end();
    }

    std::string findInputType(Context &activeContext, Context &typeScopedContext, json &element) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Expansion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#expansion-algorithm

        std::string inputType;
        std::string typeKey;

        // 11)
        // For each key and value in element ordered lexicographically by key where key
        // IRI expands to @type:
        std::vector<std::string> element_keys;
        for (json::iterator it = element.begin(); it != element.end(); ++it) {
            element_keys.push_back(it.key());
        }
        std::sort(element_keys.begin(), element_keys.end());
        for (auto & key : element_keys) {

            std::string expandedKey = ContextProcessor::expandIri(activeContext, key, false, true);

            if (expandedKey != JsonLdConsts::TYPE)
                continue;
            else if (typeKey.empty())
                typeKey = key;

            // 11.1)
            // Convert value into an array, if necessary.
            auto element_value = element[key];

            if (!element_value.is_array())
                element_value = json::array({element_value});

            // 11.2)
            // For each term which is a value of value ordered lexicographically, if term is a
            // string, and term's term definition in type-scoped context has a local context, set
            // active context to the result Context Processing algorithm, passing active
            // context, the value of the term's local context as local context, base URL from
            // the term definition for value in active context, and false for propagate.
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
                    if(termValue.contains(JsonLdConsts::BASEURL))
                        activeContext = ContextProcessor::process(activeContext, localContext,
                                                                  termValue.at(JsonLdConsts::BASEURL).get<std::string>(),
                                                                  false, false);
                    else
                        activeContext = ContextProcessor::process(activeContext, localContext, "",
                                                                  false, false);
                }
            }
        }

        // 12) (second part)
        // [...] Initialize input type to expansion of the last value of the first entry in
        // element expanding to @type (if any), ordering entries lexicographically by key. Both
        // the key and value of the matched entry are IRI expanded.
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
                inputType = ContextProcessor::expandIri(activeContext, lastValue, false, true);
            }
        }

        return inputType;
    }

    json expandValue(Context &activeContext, const std::string & activeProperty, const json& value)  {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Value Expansion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#value-expansion

        json result;
        json termDefinition = activeContext.getTermDefinition(activeProperty);

        if(termDefinition.contains(JsonLdConsts::TYPE)) {

            std::string typeMapping = termDefinition.at(JsonLdConsts::TYPE).get<std::string>();

            // 1)
            // If the active property has a type mapping in active context that is @id, and
            // the value is a string, return a new map containing a single entry where the key
            // is @id and the value is the result IRI expanding value using true for document
            // relative and false for vocab.
            if (typeMapping == JsonLdConsts::ID && value.is_string()) {
                result[JsonLdConsts::ID] = ContextProcessor::expandIri(activeContext, value.get<std::string>(), true, false);
                return result;
            }

            // 2)
            // If active property has a type mapping in active context that is @vocab, and the
            // value is a string, return a new map containing a single entry where the key is @id
            // and the value is the result of IRI expanding value using true for document relative.
            if (typeMapping == JsonLdConsts::VOCAB && value.is_string()) {
                result[JsonLdConsts::ID] = ContextProcessor::expandIri(activeContext, value.get<std::string>(), true, true);
                return result;
            }
        }

        // 3)
        // Otherwise, initialize result to a map with an @value entry whose value is set to value.
        result[JsonLdConsts::VALUE] = value;

        if(termDefinition.contains(JsonLdConsts::TYPE)) {

            std::string typeMapping = termDefinition.at(JsonLdConsts::TYPE).get<std::string>();

            // 4)
            // If active property has a type mapping in active context, other than @id, @vocab, or
            // @none, add @type to result and set its value to the value associated with the
            // type mapping.
            if (typeMapping != JsonLdConsts::ID &&
                typeMapping != JsonLdConsts::VOCAB &&
                typeMapping != JsonLdConsts::NONE) {
                result[JsonLdConsts::TYPE] = typeMapping;
            }
        }
            // 5)
            // Otherwise, if value is a string:
        else if (value.is_string()) {
            // 5.1)
            // Initialize language to the language mapping for active property in active
            // context, if any, otherwise to the default language of active context.
            std::string language;
            if (termDefinition.contains(JsonLdConsts::LANGUAGE)) {
                if(!termDefinition.at(JsonLdConsts::LANGUAGE).is_null())
                    language = termDefinition.at(JsonLdConsts::LANGUAGE).get<std::string>();
            }
            else if (!activeContext.getDefaultLanguage().empty()) {
                language = activeContext.getDefaultLanguage();
            }
            // 5.2)
            // Initialize direction to the direction mapping for active property in active
            // context, if any, otherwise to the default base direction of active context.
            std::string direction;
            if (termDefinition.contains(JsonLdConsts::DIRECTION)) {
                if(!termDefinition.at(JsonLdConsts::DIRECTION).is_null())
                    direction = termDefinition.at(JsonLdConsts::DIRECTION).get<std::string>();
            }
            else  {
                direction = activeContext.getDefaultBaseDirection();
            }
            // 5.3)
            // If language is not null, add @language to result with the value language.
            if(!language.empty())
                result[JsonLdConsts::LANGUAGE] = language;
            // 5.4)
            // If direction is not null, add @direction to result with the value direction.
            if(!direction.empty() && direction != "null")
                result[JsonLdConsts::DIRECTION] = direction;
        }
        return result;
    }

    json expandArrayElement(
            Context activeContext,
            const std::string * activeProperty,
            const json& element,
            const std::string & baseUrl,
            bool fromMap) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Expansion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#expansion-algorithm

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
            json expandedItem = ExpansionProcessor::expand(activeContext, activeProperty, item, baseUrl, fromMap);

            // 5.2.2)
            // If the container mapping of active property includes @list, and expanded item is
            // an array, set expanded item to a new map containing the entry @list where the value
            // is the original expanded item.
            if(activeProperty != nullptr) {
                auto termDefinition = activeContext.getTermDefinition(*activeProperty);
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

    std::unique_ptr<json>
    initializePropertyScopedContext(Context &activeContext, const std::string *activeProperty) {
        // 3)
        // If active property has a term definition in active context with a local
        // context, initialize property-scoped context to that local context.
        std::unique_ptr<json> propertyScopedContext;
        if(activeProperty != nullptr &&
           !activeContext.getTermDefinition(*activeProperty).is_null() &&
           activeContext.getTermDefinition(*activeProperty).contains(JsonLdConsts::LOCALCONTEXT) ) {
            propertyScopedContext.reset(new json(activeContext.getTermDefinition(*activeProperty)[JsonLdConsts::LOCALCONTEXT]));
        }
        return propertyScopedContext;
    }

    Context &
    updateActiveContext(Context &activeContext, const std::string *activeProperty, const json *propertyScopedContext) {
        // 8)
        // If property-scoped context is defined, set active context to the result of the
        // Context Processing algorithm, passing active context, property-scoped context as
        // local context, base URL from the term definition for active property in active
        // context and true for override protected.
        auto termDef = activeContext.getTermDefinition(*activeProperty);
        std::string termsBaseUrl;
        if(termDef.contains(JsonLdConsts::BASEURL))
            termsBaseUrl = termDef[JsonLdConsts::BASEURL].get<std::string>();
        activeContext = ContextProcessor::process(activeContext, *propertyScopedContext, termsBaseUrl, true);
        return activeContext;
    }

    void expandObjectElement_step13(const std::string *activeProperty, const json &element, const std::string &baseUrl,
             Context &typeScopedContext, const std::string &inputType, Context &activeContext, json &result,
             json &nests);

    void expandObjectElement_step14(Context &activeContext, const std::string *activeProperty, const json &element,
                                    const std::string &baseUrl, Context &typeScopedContext, json &result,
                                    json &nests, const std::string &inputType);

    json expandObjectElement(
            Context activeContext,
            const std::string * activeProperty,
            json element,
            const std::string & baseUrl,
            nlohmann::json * propertyScopedContext,
            bool fromMap) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the Expansion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#expansion-algorithm

        // 7)
        // If active context has a previous context, the active context is not propagated. If
        // from map is undefined or false, and element does not contain an entry expanding to
        // @value, and element does not consist of a single entry expanding to @id (where entries
        // are IRI expanded, set active context to previous context from active context, as the
        // scope of a term-scoped context does not apply when processing new node objects.
        if(activeContext.getPreviousContext() != nullptr && !fromMap) {

            bool usePrevious = true;
            std::vector<std::string> element_keys;
            for (json::iterator it = element.begin(); it != element.end(); ++it) {
                element_keys.push_back(it.key());
            }

            for (auto & key : element_keys) {

                std::string expandedKey = ContextProcessor::expandIri(activeContext, key, false, true);

                if(expandedKey == JsonLdConsts::VALUE ||
                   (element.size() == 1 && expandedKey == JsonLdConsts::ID)) {
                    usePrevious = false;
                    break;
                }
            }
            if(usePrevious)
                activeContext = * activeContext.getPreviousContext();

        }

        // 8)
        // If property-scoped context is defined, set active context to the result of the
        // Context Processing algorithm, passing active context, property-scoped context as
        // local context, base URL from the term definition for active property in active
        // context and true for override protected.
        if(propertyScopedContext != nullptr) {
            activeContext = updateActiveContext(activeContext, activeProperty, propertyScopedContext);
        }

        // 9)
        // If element contains the entry @context, set active context to the result of the
        // Context Processing algorithm, passing active context, the value of the @context entry
        // as local context and base URL.
        if (element.contains(JsonLdConsts::CONTEXT)) {
            activeContext = ContextProcessor::process(activeContext, element[JsonLdConsts::CONTEXT], baseUrl);
        }

        // 10)
        // Initialize type-scoped context to active context. This is used for expanding values
        // that may be relevant to any previous type-scoped context.
        Context typeScopedContext = activeContext;

        // 11) [Find input type...] For each key and value in element [...]
        std::string inputType = findInputType(activeContext, typeScopedContext, element);

        // 12)
        // Initialize two empty maps, result and nests.
        json result = json::object();
        json nests = json::object();

        // 13)
        // For each key and value in element, ordered lexicographically by key if ordered is true:
        expandObjectElement_step13(activeProperty, element, baseUrl, typeScopedContext, inputType, activeContext, result, nests);

        // 14)
        // For each key nesting-key in nests, ordered lexicographically if ordered is true:
        expandObjectElement_step14(activeContext, activeProperty, element, baseUrl, typeScopedContext, result,
                                   nests, inputType);

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
                if(result[JsonLdConsts::VALUE].is_null() ||
                   (result[JsonLdConsts::VALUE].is_array() && result[JsonLdConsts::VALUE].empty()))
                    return {};

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
                         (!result[JsonLdConsts::TYPE].is_string() || !JsonLdUtils::isAbsoluteIri(result[JsonLdConsts::TYPE].get<std::string>())))
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
            return {};
        }

        // Note: For step 19.1 below, the spec states that "...map which is empty, or contains
        // only..." and that seems to indicate that part of the if() statement should include
        // "result.size() == 1" but if we add that clause today then expansion test #t0046 will
        // fail. This has been noted by the JSON-LD working group and documented
        // here: https://github.com/w3c/json-ld-api/issues/496 but the latest published spec does
        // not include any changes yet. When it does, we will update this code.

        // 19)
        // If active property is null or @graph, drop free-floating values as follows:
        if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
            // 19.1)
            // If result is a map which is empty, or contains only the entries @value or
            // @list, set result to null.
            if (!result.is_null() && result.is_object() &&
                (result.empty() || result.contains(JsonLdConsts::VALUE) || result.contains(JsonLdConsts::LIST))) {
                result = json();
            }
            // 19.2)
            // Otherwise, if result is a map whose only entry is @id, set result to null. When the
            // frameExpansion flag is set, a map containing only the @id entry is retained.
            else if (!result.is_null() && !activeContext.getOptions().isFrameExpansion() && result.contains(JsonLdConsts::ID)
                     && result.size() == 1) {
                result = json();
            }
        }

        // 20)
        return result;
    }

    void expandObjectElement_step13(const std::string *activeProperty, const json &element, const std::string &baseUrl,
             Context &typeScopedContext, const std::string &inputType, Context &activeContext, json &result,
             json &nests) {

        // 13)
        // For each key and value in element, ordered lexicographically by key if ordered is true:

        std::vector<std::string> element_keys;
        for (json::const_iterator it = element.begin(); it != element.end(); ++it) {
            element_keys.push_back(it.key());
        }

        if(activeContext.getOptions().isOrdered())
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
            std::string expandedProperty = ContextProcessor::expandIri(activeContext, key, false, true);

            // 13.3)
            // If expanded property is null or it neither contains a colon (:) nor it is a
            // keyword, drop key by continuing to the next key.
            if (
                    expandedProperty.empty() ||
                    !(expandedProperty.find(':') != std::string::npos ||
                      JsonLdUtils::isKeyword(expandedProperty))) {
                continue;
            }

            json expandedValue;

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
                    if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                        throw JsonLdError(JsonLdError::CollidingKeywords,
                                          expandedProperty + " already exists in result");
                    }
                    if(expandedProperty != JsonLdConsts::INCLUDED && expandedProperty != JsonLdConsts::TYPE)
                        throw JsonLdError(JsonLdError::CollidingKeywords,
                                          expandedProperty + " already exists in result");
                }
                // 13.4.3)
                // If expanded property is @id:
                if (expandedProperty == JsonLdConsts::ID) {
                    // 13.4.3.2)
                    // [If expanded property is a string], set expanded value to the result of
                    // IRI expanding value using true for document relative and false for
                    // vocab. When the frameExpansion flag is set, expanded value will be an array
                    // of one or more of the values, with string values expanded using the IRI
                    // Expansion algorithm as above.
                    if (element_value.is_string()) {
                        expandedValue = ContextProcessor::expandIri(activeContext, element_value.get<std::string>(), true, false);
                    } else if (activeContext.getOptions().isFrameExpansion()) {
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
                                expandedValue.push_back(ContextProcessor::expandIri(activeContext, v.get<std::string>(), true, false));
                            }
                            // 13.4.3.1)
                            // If value is not a string, an invalid @id value error has been
                            // detected and processing is aborted. When the frameExpansion flag
                            // is set, value MAY be an empty map, or an array of one or more strings.
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
                    // If expanded property is @type:
                else if (expandedProperty == JsonLdConsts::TYPE) {
                    // 13.4.4.1)
                    // If value is neither a string nor an array of strings, an invalid type
                    // value error has been detected and processing is aborted. When the
                    // frameExpansion flag is set, value MAY be an empty map, or a default
                    // object where the value of @default is restricted to be an IRI. All
                    // other values mean that invalid type value error has been detected
                    // and processing is aborted.
                    if(
                            (!activeContext.getOptions().isFrameExpansion() &&
                             !element_value.is_string() &&
                             !JsonLdUtils::isArrayOfStrings(element_value))
                            ||
                            (activeContext.getOptions().isFrameExpansion() &&
                             !element_value.is_string() &&
                             !JsonLdUtils::isArrayOfStrings(element_value) &&
                             !(element_value.is_object() && element_value.empty()) &&
                             !(JsonLdUtils::isDefaultObject(element_value) &&
                               element_value[JsonLdConsts::DEFAULT].is_string() &&
                               JsonLdUtils::isIri(element_value[JsonLdConsts::DEFAULT].get<std::string>()))
                            )
                            ) {
                        throw JsonLdError(JsonLdError::InvalidTypeValue,
                                          "@type value must be a string or array of strings, or empty object or default object");
                    }

                    // 13.4.4.2)
                    // If value is an empty map, set expanded value to value.
                    if(element_value.is_object())
                        expandedValue = element_value;

                        // 13.4.4.3)
                        // Otherwise, if value is a default object, set expanded value to a new default
                        // object with the value of @default set to the result of IRI expanding value
                        // using type-scoped context for active context, and true for document relative.
                    else if(JsonLdUtils::isDefaultObject(element_value)) {
                        expandedValue = json::object();
                        expandedValue[JsonLdConsts::DEFAULT] =
                                ContextProcessor::expandIri(typeScopedContext, element_value.at(JsonLdConsts::DEFAULT).get<std::string>(), true, true);
                    }
                        // 13.4.4.4)
                        // Otherwise, set expanded value to the result of IRI expanding each of its
                        // values using type-scoped context for active context, and true for document
                        // relative.
                    else {

                        if(element_value.is_string()) {
                            expandedValue =
                                    ContextProcessor::expandIri(typeScopedContext, element_value.get<std::string>(), true, true);
                        }
                        else if(element_value.is_array()) {
                            expandedValue = json::array();
                            for(const auto& v : element_value) {
                                expandedValue.push_back(ContextProcessor::expandIri(typeScopedContext, v.get<std::string>(), true, true));
                            }
                        }

                    }
                    // 13.4.4.5)
                    // If result already has an entry for @type, prepend the value of @type in
                    // result to expanded value, transforming it into an array, if necessary.
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
                // If expanded property is @graph, set expanded value to the result of using
                // this algorithm recursively passing active context, @graph for active
                // property, value for element, base URL, and the frameExpansion and ordered
                // flags, ensuring that expanded value is an array of one or more maps.
                else if (expandedProperty == JsonLdConsts::GRAPH) {
                    expandedValue = ExpansionProcessor::expand(activeContext, &expandedProperty, element_value, baseUrl);
                    if(!expandedValue.is_array()) {
                        expandedValue = json::array({expandedValue});
                    }
                }
                // 13.4.6)
                // If expanded property is @included:
                else if (expandedProperty == JsonLdConsts::INCLUDED) {
                    // 13.4.6.1)
                    // If processing mode is json-ld-1.0, continue with the next key from element.
                    if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                        continue;
                    }
                    // 13.4.6.2)
                    // Set expanded value to the result of using this algorithm recursively passing
                    // active context, null for active property, value for element, base URL, and the
                    // frameExpansion and ordered flags, ensuring that the result is an array.
                    expandedValue = ExpansionProcessor::expand(activeContext, nullptr, element_value, baseUrl);
                    if(!expandedValue.is_array()) {
                        expandedValue = json::array({expandedValue});
                    }
                    // 13.4.6.3)
                    // If any element of expanded value is not a node object, an invalid @included
                    // value error has been detected and processing is aborted.
                    for(const auto& v : expandedValue) {
                        if(!JsonLdUtils::isNodeObject(v))
                            throw JsonLdError(JsonLdError::InvalidIncludedValue);
                    }
                    // 13.4.6.4)
                    // If result already has an entry for @included, prepend the value of @included
                    // in result to expanded value.
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
                        if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
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
                            (activeContext.getOptions().isFrameExpansion() && (JsonLdUtils::isEmptyObject(element_value) || JsonLdUtils::isArrayOfScalars(element_value)))) {
                        // 13.4.7.3)
                        // Otherwise, set expanded value to value. When the frameExpansion flag
                        // is set, expanded value will be an array of one or more string values
                        // or an array containing an empty map.
                        expandedValue = element_value;
                        if(activeContext.getOptions().isFrameExpansion()) {
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
                // If expanded property is @language:
                else if (expandedProperty == JsonLdConsts::LANGUAGE) {
                    // 13.4.8.1)
                    // If value is not a string, an invalid language-tagged string error has been
                    // detected and processing is aborted. When the frameExpansion flag is set, value
                    // MAY be an empty map or an array of zero or more strings.
                    if (element_value.is_string() ||
                        (activeContext.getOptions().isFrameExpansion() && (JsonLdUtils::isEmptyObject(element_value) ||
                                            JsonLdUtils::isEmptyArray(element_value) ||
                                            JsonLdUtils::isArrayOfStrings(element_value)))) {
                        // 13.4.8.2)
                        //  Otherwise, set expanded value to value. If value is not well-formed
                        //  according to section 2.2.9 of [BCP47], processors SHOULD issue a
                        //  warning. When the frameExpansion flag is set, expanded value will
                        //  be an array of one or more string values or an array containing
                        //  an empty map.
                        std::string v = element_value.get<std::string>();
                        if(!WellFormed::language(v)) {
                            std::cerr << "Warning: language tag " << v << " is not well-formed.\n";
                        }
                        // todo: When the frameExpansion flag is set...
                        std::transform(v.begin(), v.end(), v.begin(), &tolower);
                        expandedValue = v;
                    } else {
                        throw JsonLdError(JsonLdError::InvalidLanguageTaggedString);
                    }
                }
                // 13.4.9)
                // If expanded property is @direction:
                else if (expandedProperty == JsonLdConsts::DIRECTION) {
                    // 13.4.9.1)
                    // If processing mode is json-ld-1.0, continue with the next key from element.
                    if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                        continue;
                    }
                    // 13.4.9.2)
                    // If value is neither "ltr" nor "rtl", an invalid base direction error has been
                    // detected and processing is aborted. When the frameExpansion flag is set, value
                    // MAY be an empty map or an array of zero or more strings.
                    if ((element_value.is_string() && (element_value == "ltr" || element_value == "rtl")) ||
                        (activeContext.getOptions().isFrameExpansion() && (JsonLdUtils::isEmptyObject(element_value) ||
                                            JsonLdUtils::isEmptyArray(element_value) ||
                                            JsonLdUtils::isArrayOfStrings(element_value)))) {
                        // 13.4.9.3)
                        // Otherwise, set expanded value to value. When the frameExpansion flag is
                        // set, expanded value will be an array of one or more string values or an
                        // array containing an empty map.
                        expandedValue = element_value;
                        if(activeContext.getOptions().isFrameExpansion()) {
                            if(!expandedValue.is_array()) {
                                expandedValue = json::array({expandedValue});
                            }
                        }
                    } else {
                        throw JsonLdError(JsonLdError::InvalidBaseDirection);
                    }
                }
                    // 13.4.10)
                    // If expanded property is @index:
                else if (expandedProperty == JsonLdConsts::INDEX) {
                    // 13.4.10.1
                    // If value is not a string, an invalid @index value error has been detected
                    // and processing is aborted.
                    if (!element_value.is_string()) {
                        throw JsonLdError(JsonLdError::InvalidIndexValue);
                    }
                    // 13.4.10.2
                    // Otherwise, set expanded value to value.
                    expandedValue = element_value;
                }
                    // 13.4.11)
                    // If expanded property is @list:
                else if (expandedProperty == JsonLdConsts::LIST) {
                    // 13.4.11.1)
                    // If active property is null or @graph, continue with the next key from
                    // element to remove the free-floating list.
                    if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
                        continue;
                    }
                    // 13.4.11.2)
                    // Otherwise, initialize expanded value to the result of using this algorithm
                    // recursively passing active context, active property, value for element, base
                    // URL, and the frameExpansion and ordered flags, ensuring that the result is
                    // an array.
                    expandedValue = ExpansionProcessor::expand(activeContext, activeProperty, element_value, baseUrl);
                    if (!expandedValue.is_array()) {
                        expandedValue = json::array({expandedValue});
                    }
                }
                    // 13.4.12)
                    // If expanded property is @set, set expanded value to the result of using this
                    // algorithm recursively, passing active context, active property, value for
                    // element, base URL, and the frameExpansion and ordered flags.
                else if (expandedProperty == JsonLdConsts::SET) {
                    expandedValue = ExpansionProcessor::expand(activeContext, activeProperty, element_value, baseUrl);
                }
                    // 13.4.13)
                    // If expanded property is @reverse:
                else if (expandedProperty == JsonLdConsts::REVERSE) {
                    // 13.4.13.1)
                    // If value is not a map, an invalid @reverse value error has been detected and
                    // processing is aborted.
                    if (!element_value.is_object()) {
                        throw JsonLdError(JsonLdError::InvalidReverseValue,
                                          "@reverse value must be an object");
                    }
                    // 13.4.13.2)
                    // Otherwise initialize expanded value to the result of using this algorithm
                    // recursively, passing active context, @reverse as active property, value as
                    // element, base URL, and the frameExpansion and ordered flags.
                    {
                        std::string currentProperty = JsonLdConsts::REVERSE;
                        expandedValue = ExpansionProcessor::expand(activeContext, &currentProperty, element_value, baseUrl);
                    }
                    // 13.4.13.3)
                    // If expanded value contains an @reverse entry, i.e., properties that are
                    // reversed twice, execute for each of its property and item the following steps:
                    if (expandedValue.contains(JsonLdConsts::REVERSE)) {
                        // 13.4.13.3.1)
                        // Use add value to add item to the property entry in result using true for as array.
                        for (const auto& entry : expandedValue[JsonLdConsts::REVERSE].items()) {
                            JsonLdUtils::addValue(result, entry.key(), entry.value(), true);
                        }
                    }
                    // 13.4.13.4)
                    // If expanded value contains an entry other than @reverse:
                    if (expandedValue.size() > (expandedValue.contains(JsonLdConsts::REVERSE) ? 1 : 0)) {
                        // 13.4.13.4.1)
                        // Set reverse map to the value of the @reverse entry in result, initializing
                        // it to an empty map, if necessary.
                        if (!result.contains(JsonLdConsts::REVERSE)) {
                            result[JsonLdConsts::REVERSE] = json::object();
                        }
                        auto & reverseMap = result[JsonLdConsts::REVERSE];

                        // 13.4.13.4.2)
                        // For each property and items in expanded value other than @reverse:
                        for (const auto& entry : expandedValue.items())
                        {
                            if(entry.key() == JsonLdConsts::REVERSE)
                                continue;

                            // 13.4.13.4.2.1)
                            // For each item in items:
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
                    // Continue with the next key from element.
                    continue;
                }

                // 13.4.14)
                // If expanded property is @nest, add key to nests, initializing it to an empty
                // array, if necessary. Continue with the next key from element.
                if (expandedProperty == JsonLdConsts::NEST) {
                    if (!nests.contains(key)) {
                        nests[key] = json::array();
                    }
                    continue;
                }

                // 13.4.15)
                // When the frameExpansion flag is set, if expanded property is any other framing
                // keyword (@default, @embed, @explicit, @omitDefault, or @requireAll), set expanded
                // value to the result of performing the Expansion Algorithm recursively, passing
                // active context, active property, value for element, base URL, and the
                // frameExpansion and ordered flags.
                if (activeContext.getOptions().isFrameExpansion() &&
                    (expandedProperty == JsonLdConsts::DEFAULT ||
                     expandedProperty == JsonLdConsts::EMBED ||
                     expandedProperty == JsonLdConsts::EXPLICIT ||
                     expandedProperty == JsonLdConsts::OMIT_DEFAULT ||
                     expandedProperty == JsonLdConsts::REQUIRE_ALL)) {
                    expandedValue = ExpansionProcessor::expand(activeContext, activeProperty, element_value, baseUrl);
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
                // Continue with the next key from element.
                continue;
            }

            // 13.5)
            // Initialize container mapping to key's container mapping in active context.
            auto keyTermDefinition = activeContext.getTermDefinition(key);

            json containerMapping;
            if(keyTermDefinition.contains(JsonLdConsts::CONTAINER)) {
                containerMapping = keyTermDefinition[JsonLdConsts::CONTAINER];
            }

            // 13.6)
            // If key's term definition in active context has a type mapping of @json, set expanded
            // value to a new map, set the entry @value to value, and set the entry @type to @json.
            if(keyTermDefinition.contains(JsonLdConsts::TYPE) &&
               keyTermDefinition.at(JsonLdConsts::TYPE) == JsonLdConsts::JSON) {
                expandedValue = json::object();
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
                json direction = activeContext.getDefaultBaseDirection();
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
                if(activeContext.getOptions().isOrdered())
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
                        json v = json::object();
                        std::transform(language.begin(), language.end(), language.begin(), &tolower);
                        v[JsonLdConsts::VALUE] = item;
                        v[JsonLdConsts::LANGUAGE] = language;

                        // note: The spec text above says "If item is neither..." but it should actually
                        // say "If language is neither..."
                        // See https://github.com/w3c/json-ld-api/issues/563
                        if(language != JsonLdConsts::NONE && !WellFormed::language(language)) {
                            std::cerr << "Warning: language tag " << language << " is not well-formed.\n";
                        }

                        // 13.7.4.2.4)
                        // If language is @none, or expands to @none, remove @language from v.
                        if(language == JsonLdConsts::NONE ||
                                ContextProcessor::expandIri(activeContext, language, false, true) == JsonLdConsts::NONE) {
                            v.erase(JsonLdConsts::LANGUAGE);
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
                if(activeContext.getOptions().isOrdered())
                    std::sort(indexKeys.begin(), indexKeys.end());

                for ( const auto& index : indexKeys) {
                    json indexValue = element_value[index];

                    // 13.8.3.1)
                    // If container mapping includes @id or @type, initialize map context to the previous context
                    // from active context if it exists, otherwise, set map context to active context.
                    Context mapContext = activeContext;
                    if(arrayContains(containerMapping, JsonLdConsts::TYPE) ||
                       arrayContains(containerMapping, JsonLdConsts::ID)) {
                        if(activeContext.getPreviousContext())
                            mapContext = *activeContext.getPreviousContext();
                    }

                    // 13.8.3.2)
                    // If container mapping includes @type and index's term definition in map
                    // context has a local context, update map context to the result of the
                    // Context Processing algorithm, passing map context as active context, the
                    // value of the index's local context as local context and base URL from
                    // the term definition for index in map context.
                    if(arrayContains(containerMapping, JsonLdConsts::TYPE)) {
                        auto indexTermDefinition = mapContext.getTermDefinition(index);
                        if(indexTermDefinition.contains(JsonLdConsts::LOCALCONTEXT)) {
                            mapContext = ContextProcessor::process(mapContext,
                                    indexTermDefinition.at(JsonLdConsts::LOCALCONTEXT),
                                    indexTermDefinition.at(JsonLdConsts::BASEURL).get<std::string>());
                        }
                    }
                        // 13.8.3.3)
                        // Otherwise, set map context to active context.
                    else
                        mapContext = activeContext;

                    // 13.8.3.4)
                    // Initialize expanded index to the result of IRI expanding index.
                    auto expandedIndex = ContextProcessor::expandIri(activeContext, index, false, true);

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
                    indexValue = ExpansionProcessor::expand(mapContext, &key, indexValue, baseUrl, true);

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
                            auto reExpandedIndex = expandValue(activeContext, indexKey, index);

                            // 13.8.3.7.2.2)
                            // Initialize expanded index key to the result of IRI expanding index key.
                            auto expandedIndexKey = ContextProcessor::expandIri(activeContext, indexKey, false, true);

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
                            auto expandedIndex2 = ContextProcessor::expandIri(activeContext, index, true, false);
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
                expandedValue = ExpansionProcessor::expand(activeContext, &key, element_value, baseUrl);
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
            if (activeContext.isReverseProperty(key)) {
                // 13.13.1)
                // If result has no @reverse entry, create one and initialize its value to an empty map.
                if (!result.contains(JsonLdConsts::REVERSE)) {
                    result[JsonLdConsts::REVERSE] = json::object();
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
                for ( const auto& item : expandedValue) {
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
                // Otherwise, key is not a reverse property. Use add value to add expanded value to
                // the expanded property entry in result using true for as array.
            else {
                JsonLdUtils::addValue(result, expandedProperty, expandedValue, true);
            }
        }
    }

    void expandObjectElement_step14(Context &activeContext, const std::string *activeProperty, const json &element,
                                    const std::string &baseUrl, Context &typeScopedContext, json &result,
                                    json &nests, const std::string &inputType) {
        // 14)
        // For each key nesting-key in nests, ordered lexicographically if ordered is true:
        std::vector<std::string> nestKeys;
        for(auto& el : nests.items()) {
            nestKeys.push_back(el.key());
        }
        if(activeContext.getOptions().isOrdered())
            std::sort(nestKeys.begin(), nestKeys.end());

        for ( const auto& nestingKey : nestKeys) {

            // 14.1)
            // Initialize nested values to the value of nesting-key in element, ensuring
            // that it is an array.
            if(!element.contains(nestingKey))
                throw JsonLdError(JsonLdError::InvalidNestValue);
            auto nestedValues = element[nestingKey];
            if(!nestedValues.is_array())
                nestedValues = json::array({nestedValues});

            // 14.2)
            // For each nested value in nested values:
            for(const auto& nestedValue : nestedValues) {

                // 14.2.1)
                // If nested value is not a map, or any key within nested value expands to
                // @value, an invalid @nest value error has been detected and processing is aborted.
                if(!nestedValue.is_object()) {
                    throw JsonLdError(JsonLdError::InvalidNestValue);
                }

                for(const auto& el : nestedValue.items()) {
                    if(ContextProcessor::expandIri(activeContext, el.key(), false, true) == JsonLdConsts::VALUE)
                        throw JsonLdError(JsonLdError::InvalidNestValue);
                }

                // 14.2.2)
                // Recursively repeat steps 3, 8, 13, and 14 using nesting-key for active
                // property, and nested value for element.

                // Note:
                // Steps 3 and 8 may update the active context based on a property-scoped context
                // associated with nesting-key. Updates to active context are restricted to the
                // recursive operation, and do not propagate to subsequent iterations on nested
                // values and nesting-key.

                // 3)
                // If active property has a term definition in active context with a local
                // context, initialize property-scoped context to that local context.
                Context copyActiveContext = activeContext;
                std::unique_ptr<json> localPropertyScopedContext = initializePropertyScopedContext(copyActiveContext, activeProperty);

                // 8)
                // If property-scoped context is defined, set active context to the result of the
                // Context Processing algorithm, passing active context, property-scoped context as
                // local context, base URL from the term definition for active property in active
                // context and true for override protected.
                if(localPropertyScopedContext != nullptr) {
                    copyActiveContext = updateActiveContext(copyActiveContext, activeProperty, localPropertyScopedContext.get());
                }

                json localNests = json::object();

                // 13)
                // For each key and value in element, ordered lexicographically by key if ordered is true:
                expandObjectElement_step13(&nestingKey, nestedValue, baseUrl, typeScopedContext, inputType, copyActiveContext, result, localNests);

                // 14)
                // For each key nesting-key in nests, ordered lexicographically if ordered is true:
                expandObjectElement_step14(copyActiveContext, &nestingKey, nestedValue, baseUrl, typeScopedContext, result,
                                           localNests, inputType);

            }
        }
    }

}

nlohmann::json ExpansionProcessor::expand(
        Context activeContext,
        const std::string *activeProperty,
        nlohmann::json element,
        const std::string &baseUrl,
        bool fromMap)
{
    // Comments in this function are labeled with numbers that correspond to sections
    // from the description of the Expansion algorithm.
    // See: https://www.w3.org/TR/json-ld11-api/#expansion-algorithm

    // 1)
    // If element is null, return null.
    if (element.is_null()) {
        return element;
    }

    // 2)
    // If active property is @default, initialize the frameExpansion flag to false.
    if (activeProperty != nullptr && *activeProperty == JsonLdConsts::DEFAULT) {
        //todo: activeContext.getOptions().setFrameExpansion(false);
    }

    // 3)
    // If active property has a term definition in active context with a local
    // context, initialize property-scoped context to that local context.
    std::unique_ptr<json> propertyScopedContext = initializePropertyScopedContext(activeContext, activeProperty);

    // 5)
    // If element is an array
    if(element.is_array())
        return expandArrayElement(activeContext, activeProperty, element, baseUrl, fromMap);
        // 6)
        // [If] element is a map.
    else if(element.is_object())
        return expandObjectElement(activeContext, activeProperty, element, baseUrl, propertyScopedContext.get(), fromMap);
        // 4)
        // [else] element is a scalar
    else
    {
        // 4.1)
        // If active property is null or @graph, drop the free-floating scalar by returning null.
        if (activeProperty == nullptr || *activeProperty == JsonLdConsts::GRAPH) {
            return {};
        }

        // 4.2)
        // If property-scoped context is defined, set active context to the result of the
        // Context Processing algorithm, passing active context, property-scoped context
        // as local context, and base URL from the term definition for active property in
        // active context.
        if(propertyScopedContext != nullptr) {
            auto termDef = activeContext.getTermDefinition(*activeProperty);
            std::string termsBaseUrl;
            if(termDef.contains(JsonLdConsts::BASEURL))
                termsBaseUrl = termDef[JsonLdConsts::BASEURL].get<std::string>();
            activeContext = ContextProcessor::process(activeContext, *propertyScopedContext, termsBaseUrl);
        }

        // 4.3)
        // Return the result of the Value Expansion algorithm, passing the active
        // context, active property, and element as value.
        return expandValue(activeContext, *activeProperty, element);
    }

}
