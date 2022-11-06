#include "jsonld-cpp/ContextProcessor.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/JsonLdUrl.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/Uri.h"
#include <set>

using nlohmann::json;

namespace {

    // 1.1 spec provides for returning error if too many remote contexts are loaded
    const int MAX_REMOTE_CONTEXTS = 256;

    std::string expandIri(Context & activeContext,
                          std::string value, bool relative, bool vocab,
                          const json& localContext, std::map<std::string, bool> & defined);

    Context process(
            const Context & activeContext,
            const nlohmann::json & localContext,
            const std::string &baseURL,
            std::vector<std::string> remoteContexts,
            bool overrideProtected,
            bool propagate,
            bool validateScopedContext);

        /**
         * Create Term Definition Algorithm
         *
         * https://www.w3.org/TR/json-ld11-api/#create-term-definition
         *
         */
    void createTermDefinition(
            Context & activeContext,
            nlohmann::json localContext,
            const std::string& term,
            std::map<std::string, bool> & defined,
            const std::string& baseURL = "",
            bool isProtected = false,
            bool ioverrideProtected = false,
            std::vector<std::string> iremoteContexts = std::vector<std::string>()) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the create term definition algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#create-term-definition

        // 1)
        // If defined contains the entry term and the associated value is true (indicating
        // that the term definition has already been created), return. Otherwise, if the
        // value is false, a cyclic IRI mapping error has been detected and processing is aborted.
        if (defined.count(term)) {
            if (defined[term]) {
                return;
            }
            throw JsonLdError(JsonLdError::CyclicIriMapping, term);
        }

        // 2)
        // If term is the empty string (""), an invalid term definition error has been detected
        // and processing is aborted.
        if (term.empty())
            throw JsonLdError(JsonLdError::InvalidTermDefinition, term);

        // 2)
        // Otherwise, set the value associated with defined's term entry to false. This indicates
        // that the term definition is now being created but is not yet complete.
        defined[term] = false;

        // 3)
        // Initialize value to a copy of the value associated with the entry term in local context.
        auto value = localContext.at(term);

        // 4)
        // If term is @type, and processing mode is json-ld-1.0, a keyword redefinition error has
        // been detected and processing is aborted.
        if(term == JsonLdConsts::TYPE) {
            if(activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                throw JsonLdError(JsonLdError::KeywordRedefinition, term);
            }
            // At this point, value MUST be a map with only either or both of the following entries:
            // * An entry for @container with value @set.
            // * An entry for @protected.
            // Any other value means that a keyword redefinition error has been detected and
            // processing is aborted.
            if(value.is_object()) {
                if(value.size() == 1 &&
                   value.contains(JsonLdConsts::CONTAINER)) {
                    auto container = value.at(JsonLdConsts::CONTAINER);
                    if(!container.is_string() || container != JsonLdConsts::SET) {
                        throw JsonLdError(JsonLdError::KeywordRedefinition, term);
                    }
                } else if(value.size() == 2 &&
                          value.contains(JsonLdConsts::CONTAINER) &&
                          value.contains(JsonLdConsts::PROTECTED)) {
                    auto container = value.at(JsonLdConsts::CONTAINER);
                    if(!container.is_string() || container != JsonLdConsts::SET) {
                        throw JsonLdError(JsonLdError::KeywordRedefinition, term);
                    }
                } else if (value.size() != 1 || !value.contains(JsonLdConsts::PROTECTED)) {
                    throw JsonLdError(JsonLdError::KeywordRedefinition, term);
                }

            }
            else {
                throw JsonLdError(JsonLdError::KeywordRedefinition, term);
            }

        }
            // 5)
            // Otherwise, since keywords cannot be overridden, term MUST NOT be a keyword and
            // a keyword redefinition error has been detected and processing is aborted.
        else if (JsonLdUtils::isKeyword(term)) {
            throw JsonLdError(JsonLdError::KeywordRedefinition, term);
        } else if (JsonLdUtils::isKeywordForm(term)){
            // If term has the form of a keyword (i.e., it matches the ABNF rule "@"1*ALPHA from [RFC5234]),
            // return; processors SHOULD generate a warning.
            return;
            // todo: emit a warning
        }

        // 6)
        // Initialize previous definition to any existing term definition for term in
        // active context, removing that term definition from active context.
        json previousDefinition;
        if (activeContext.termDefinitions.contains(term)) {
            previousDefinition = activeContext.termDefinitions[term];
            activeContext.termDefinitions.erase(term);
        }

        bool simpleTerm = false;

        // 7)
        // If value is null, convert it to a map consisting of a single entry whose key
        // is @id and whose value is null.
        if(value.is_null()) {
            value = { { JsonLdConsts::ID, nullptr} };
        }

            // 8)
            // Otherwise, if value is a string, convert it to a map consisting of a single entry whose
            // key is @id and whose value is value. Set simple term to true.
        else if (value.is_string()) {
            value = { { JsonLdConsts::ID, value} };
            simpleTerm = true;
        }

            // 9)
            // Otherwise, value MUST be a map, if not, an invalid term definition error has been
            // detected and processing is aborted. Set simple term to false.
        else if (!(value.is_object())) {
            throw JsonLdError(JsonLdError::InvalidTermDefinition);
        }

        // 10)
        // Create a new term definition, definition, initializing prefix flag to false, protected
        // to protected, and reverse property to false.
        auto definition = json::object();
        definition[JsonLdConsts::IS_PREFIX_FLAG] = false;
        definition[JsonLdConsts::IS_PROTECTED_FLAG] = isProtected;
        definition[JsonLdConsts::IS_REVERSE_PROPERTY_FLAG] = false;

        // 11)
        // If value has an @protected entry, set the protected flag in definition to the value
        // of this entry. If the value of @protected is not a boolean, an invalid @protected value
        // error has been detected and processing is aborted. If processing mode is json-ld-1.0, an
        // invalid term definition has been detected and processing is aborted.
        if (value.contains(JsonLdConsts::PROTECTED)) {
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition);
            }

            if (!value.at(JsonLdConsts::PROTECTED).is_boolean()) {
                throw JsonLdError(JsonLdError::InvalidProtectedValue);
            }

            definition[JsonLdConsts::IS_PROTECTED_FLAG] = value.at(JsonLdConsts::PROTECTED).get<bool>();
        }

        // 12)
        // If value contains the entry @type:
        if (value.contains(JsonLdConsts::TYPE)) {
            // 12.1)
            // Initialize type to the value associated with the @type entry, which MUST be a
            // string. Otherwise, an invalid type mapping error has been detected and processing
            // is aborted.
            auto type = value.at(JsonLdConsts::TYPE);
            if (!(type.is_string())) {
                throw JsonLdError(JsonLdError::InvalidTypeMapping);
            }
            std::string typeStr = type.get<std::string>();

            // 12.2)
            // Set type to the result of IRI expanding type, using local context, and defined.
            typeStr = expandIri(activeContext, typeStr, false, true, localContext, defined);

            // 12.3)
            // If the expanded type is @json or @none, and processing mode is json-ld-1.0, an invalid
            // type mapping error has been detected and processing is aborted.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                if(typeStr == JsonLdConsts::JSON || typeStr == JsonLdConsts::NONE) {
                    throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
                }
            }

            // 12.4)
            // Otherwise, if the expanded type is neither @id, nor @json, nor @none, nor @vocab, nor
            // an IRI, an invalid type mapping error has been detected and processing is aborted.
            if(!(typeStr == JsonLdConsts::ID ||
                 typeStr == JsonLdConsts::JSON ||
                 typeStr == JsonLdConsts::NONE ||
                 typeStr == JsonLdConsts::VOCAB ||
                 JsonLdUtils::isAbsoluteIri(typeStr))) {
                throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
            }

            // 12.5)
            // Set the type mapping for definition to type.
            definition[JsonLdConsts::TYPE] = typeStr;
        }

        // 13)
        // If value contains the entry @reverse:
        if (value.contains(JsonLdConsts::REVERSE)) {
            // 13.1)
            // If value contains @id or @nest, entries, an invalid reverse property error
            // has been detected and processing is aborted.
            if (value.contains(JsonLdConsts::ID) || value.contains(JsonLdConsts::NEST)) {
                throw JsonLdError(JsonLdError::InvalidReverseProperty);
            }

            // 13.2)
            // If the value associated with the @reverse entry is not a string, an invalid IRI
            // mapping error has been detected and processing is aborted.
            auto reverse = value.at(JsonLdConsts::REVERSE);
            if (!(reverse.is_string())) {
                throw JsonLdError(JsonLdError::InvalidIriMapping,
                                  "Expected String for @reverse value.");
            }
            std::string reverseStr = reverse.get<std::string>();

            // 13.3)
            // If the value associated with the @reverse entry is a string having the form of
            // a keyword (i.e., it matches the ABNF rule "@"1*ALPHA from [RFC5234]), return;
            // processors SHOULD generate a warning.
            if(JsonLdUtils::isKeywordForm(reverseStr)) {
                // todo: emit a warning
                return;
            }

            // 13.4)
            // Otherwise, set the IRI mapping of definition to the result of IRI expanding
            // the value associated with the @reverse entry, using local context, and
            // defined. If the result does not have the form of an IRI or a blank node
            // identifier, an invalid IRI mapping error has been detected and processing is aborted.
            reverseStr = expandIri(activeContext, reverseStr, false, true, localContext, defined);
            //if (!JsonLdUtils::isAbsoluteIri(reverseStr) || !BlankNodeNames::hasFormOfBlankNodeName(reverseStr)) {
            //todo: if we add call for hasFormOfBlankNodeName as the description seems to say, we fail more tests
            if (!JsonLdUtils::isAbsoluteIri(reverseStr)) {
                throw JsonLdError(JsonLdError::InvalidIriMapping,
                                  "Non-absolute @reverse IRI: " + reverseStr);
            }
            definition[JsonLdConsts::ID] = reverseStr;

            // 13.5)
            // If value contains an @container entry, set the container mapping of definition
            // to an array containing its value; if its value is neither @set, nor @index, nor
            // null, an invalid reverse property error has been detected (reverse properties only
            // support set- and index-containers) and processing is aborted.
            if (value.contains(JsonLdConsts::CONTAINER)) {
                auto container = value.at(JsonLdConsts::CONTAINER);
                if (container == JsonLdConsts::SET || container == JsonLdConsts::INDEX || container.is_null()) {
                    definition[JsonLdConsts::CONTAINER] = json::array({container});
                } else {
                    throw JsonLdError(JsonLdError::InvalidReverseProperty,
                                      "reverse properties only support set- and index-containers");
                }
            }

            // 13.6)
            // Set the reverse property flag of definition to true.
            definition[JsonLdConsts::REVERSE] = true;

            // 13.7)
            // Set the term definition of term in active context to definition and the value
            // associated with defined's entry term to true and return.
            activeContext.termDefinitions[term] = definition;
            defined[term] = true;
            return;
        }

        // 14)
        // If value contains the entry @id and its value does not equal term
        if (value.contains(JsonLdConsts::ID) &&
            (!value.at(JsonLdConsts::ID).is_string() || term != value.at(JsonLdConsts::ID))) {

            auto id = value.at(JsonLdConsts::ID);

            // 14.1)
            // If the @id entry of value is null, the term is not used for IRI expansion, but is
            // retained to be able to detect future redefinitions of this term.
            if(!id.is_null()) {

                // 14.2)
                // otherwise

                // 14.2.1)
                // If the value associated with the @id entry is not a string, an invalid IRI
                // mapping error has been detected and processing is aborted.
                if (!id.is_string()) {
                    throw JsonLdError(JsonLdError::InvalidIriMapping,
                                      "expected value of @id to be a string");
                }

                std::string idStr = id.get<std::string>();

                // 14.2.2)
                // If the value associated with the @id entry is not a keyword, but has the
                // form of a keyword (i.e., it matches the ABNF rule "@"1*ALPHA from [RFC5234]),
                // return; processors SHOULD generate a warning.
                if(!JsonLdUtils::isKeyword(idStr) && JsonLdUtils::isKeywordForm(idStr)) {
                    // todo: emit a warning
                    return;
                }

                // 14.2.3)
                // Otherwise, set the IRI mapping of definition to the result of IRI expanding
                // the value associated with the @id entry, using local context, and defined.
                idStr = expandIri(activeContext, idStr, false, true, localContext, defined);
                // If the resulting IRI mapping is neither a keyword, nor an IRI, nor a blank node
                // identifier, an invalid IRI mapping error has been detected and processing is
                // aborted; if it equals @context, an invalid keyword alias error has been detected
                // and processing is aborted.
                if (JsonLdUtils::isKeyword(idStr) || JsonLdUtils::isAbsoluteIri(idStr) ||
                        BlankNodeNames::hasFormOfBlankNodeName(idStr)) {
                    if (idStr == JsonLdConsts::CONTEXT) {
                        throw JsonLdError(JsonLdError::InvalidKeywordAlias, "cannot alias @context");
                    }
                    definition[JsonLdConsts::ID] = idStr;
                } else {
                    throw JsonLdError(JsonLdError::InvalidIriMapping,
                                      "resulting IRI mapping should be a keyword, absolute IRI or blank node");
                }

                // 14.2.4)
                // If the term contains a colon (:) anywhere but as the first or last character
                // of term, or if it contains a slash (/) anywhere:
                if(term.substr(0, term.size()-1).find(':',1) != std::string::npos ||
                   term.find('/') != std::string::npos) {

                    // 14.2.4.1)
                    // Set the value associated with defined's term entry to true.
                    defined[term] = true;

                    // 14.2.4.2)
                    // If the result of IRI expanding term using local context, and defined, is
                    // not the same as the IRI mapping of definition, an invalid IRI mapping error
                    // has been detected and processing is aborted.
                    std::string expandedTerm = expandIri(activeContext, term, false, true, localContext, defined);
                    if(expandedTerm != definition[JsonLdConsts::ID]) {
                        throw JsonLdError(JsonLdError::InvalidIriMapping,
                                          "expanded term is not the same as IRI mapping");
                    }
                }

                // 14.2.5)
                // If term contains neither a colon (:) nor a slash (/), simple term is true,
                // and if the IRI mapping of definition is either an IRI ending with a gen-delim
                // character, or a blank node identifier, set the prefix flag in definition to true.
                if (term.find(':') == std::string::npos &&
                    term.find('/') == std::string::npos &&
                    simpleTerm &&
                    ((JsonLdUtils::isAbsoluteIri(definition[JsonLdConsts::ID].get<std::string>()) &&
                      JsonLdUtils::iriEndsWithGeneralDelimiterCharacter(definition[JsonLdConsts::ID].get<std::string>())) ||
                            BlankNodeNames::hasFormOfBlankNodeName(definition[JsonLdConsts::ID].get<std::string>())))
                {
                    definition[JsonLdConsts::IS_PREFIX_FLAG] = true;
                }
            }
        }
            // 15)
            // Otherwise if the term contains a colon (:) anywhere after the first character:
        else if (term.find(':', 1) != std::string::npos) {
            // todo: maybe need a new "compact uri" class? See https://www.w3.org/TR/curie/ ...
            // 15.1)
            // If term is a compact IRI with a prefix that is an entry in local context
            // a dependency has been found. Use this algorithm recursively passing active
            // context, local context, the prefix as term, and defined.
            auto colIndex = term.find(':');
            std::string prefix(term, 0, colIndex);
            std::string suffix(term, colIndex + 1);
            if (localContext.contains(prefix)) {
                createTermDefinition(activeContext, localContext, prefix, defined);
            }
            // 15.2)
            // If term's prefix has a term definition in active context, set the IRI mapping of
            // definition to the result of concatenating the value associated with the prefix's
            // IRI mapping and the term's suffix.
            if (activeContext.termDefinitions.contains(prefix)) {
                auto id = activeContext.termDefinitions.at(prefix).at(JsonLdConsts::ID);
                id = id.get<std::string>() + suffix;
                definition[JsonLdConsts::ID] = id;
            }
                // 15.3)
                // Otherwise, term is an IRI or blank node identifier. Set the IRI mapping of
                // definition to term.
            else {
                definition[JsonLdConsts::ID] = term;
            }
        }
            // 16)
            // Otherwise if the term contains a slash (/):
        else if (term.find('/') != std::string::npos) {
            // 16.1
            // Term is a relative IRI reference.
            assert(JsonLdUtils::isRelativeIri(term));

            // 16.2)
            // Set the IRI mapping of definition to the result of IRI expanding term. If the
            // resulting IRI mapping is not an IRI, an invalid IRI mapping error has been detected
            // and processing is aborted.
            std::string expandedTerm = expandIri(activeContext, term, false, true, localContext, defined);
            definition[JsonLdConsts::ID] = expandedTerm;
            if(!JsonLdUtils::isAbsoluteIri(definition[JsonLdConsts::ID].get<std::string>())) {
                throw JsonLdError(JsonLdError::InvalidIriMapping,
                                  "expanded term is not an IRI");
            }
        }
            // 17)
            // Otherwise, if term is @type, set the IRI mapping of definition to @type.
        else if (term == JsonLdConsts::TYPE) {
            definition[JsonLdConsts::ID] = JsonLdConsts::TYPE;
        }
            // 18)
            // Otherwise, if active context has a vocabulary mapping, the IRI mapping of definition
            // is set to the result of concatenating the value associated with the vocabulary mapping
            // and term. If it does not have a vocabulary mapping, an invalid IRI mapping error been
            // detected and processing is aborted.
        else if (!activeContext.getVocabularyMapping().empty()) {
            definition[JsonLdConsts::ID] = activeContext.getVocabularyMapping() + term;
        }
        else {
            throw JsonLdError(JsonLdError::InvalidIriMapping,
                              "relative term definition without vocab mapping");
        }

        // 19)
        // If value contains the entry @container:
        if (value.contains(JsonLdConsts::CONTAINER)) {

            // 19.1) (partial)
            // Initialize container to the value associated with the @container entry
            auto container = value.at(JsonLdConsts::CONTAINER);

            // 19.2)
            // If the container value is @graph, @id, or @type, or is otherwise not a string,
            // generate an invalid container mapping error and abort processing if processing
            // mode is json-ld-1.0.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                if (!container.is_string())
                    throw JsonLdError(JsonLdError::InvalidContainerMapping,
                                      "@container must be a string");
                if(container != JsonLdConsts::LIST &&
                   container != JsonLdConsts::SET &&
                   container != JsonLdConsts::INDEX &&
                   container != JsonLdConsts::LANGUAGE) {
                    throw JsonLdError(JsonLdError::InvalidContainerMapping,
                                      "@container must be either @list, @set, @index, or @language");
                }
            }

            // 19.1)
            // ... container ... MUST be either @graph, @id, @index, @language, @list, @set,
            // @type, or an array containing exactly any one of those keywords, an array
            // containing @graph and either @id or @index optionally including @set, or an
            // array containing a combination of @set and any of @index, @graph, @id, @type,
            // @language in any order. Otherwise, an invalid container mapping has been
            // detected and processing is aborted.
            if(container.is_array() && container.size() == 1)
                container = container.at(0);

            if(!JsonLdUtils::containsOrEquals(container, JsonLdConsts::GRAPH) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::ID) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::INDEX) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::LANGUAGE) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::LIST) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::SET) &&
               !JsonLdUtils::containsOrEquals(container, JsonLdConsts::TYPE)) {
                throw JsonLdError(JsonLdError::InvalidContainerMapping);
            }

            if(container.is_array()) {

                if (container.size() > 3)
                    throw JsonLdError(JsonLdError::InvalidContainerMapping);

                if (JsonLdUtils::containsOrEquals(container, JsonLdConsts::GRAPH) &&
                    JsonLdUtils::containsOrEquals(container, JsonLdConsts::ID)) {
                    if (container.size() != 2 && !JsonLdUtils::containsOrEquals(container, JsonLdConsts::SET))
                        throw JsonLdError(JsonLdError::InvalidContainerMapping);
                }

                else if (JsonLdUtils::containsOrEquals(container, JsonLdConsts::GRAPH) &&
                         JsonLdUtils::containsOrEquals(container, JsonLdConsts::INDEX)) {
                    if (container.size() != 2 && !JsonLdUtils::containsOrEquals(container, JsonLdConsts::SET))
                        throw JsonLdError(JsonLdError::InvalidContainerMapping);
                }

                else if (container.size() > 2)
                    throw JsonLdError(JsonLdError::InvalidContainerMapping);

                if (JsonLdUtils::containsOrEquals(container, JsonLdConsts::SET)) {
                    if (!JsonLdUtils::containsOrEquals(container, JsonLdConsts::GRAPH) &&
                        !JsonLdUtils::containsOrEquals(container, JsonLdConsts::ID) &&
                        !JsonLdUtils::containsOrEquals(container, JsonLdConsts::INDEX) &&
                        !JsonLdUtils::containsOrEquals(container, JsonLdConsts::LANGUAGE) &&
                        !JsonLdUtils::containsOrEquals(container, JsonLdConsts::TYPE)) {
                        throw JsonLdError(JsonLdError::InvalidContainerMapping);
                    }
                }
            }
            else if(!container.is_string())
                throw JsonLdError(JsonLdError::InvalidContainerMapping,
                                  "@container must be a string or array");

            // 19.3
            // Set the container mapping of definition to container coercing to an array, if necessary.
            if(container.is_array())
                definition[JsonLdConsts::CONTAINER] = container;
            else {
                definition[JsonLdConsts::CONTAINER] = json::array();
                definition[JsonLdConsts::CONTAINER].push_back(container);
            }

            // 19.4)
            // If the container mapping of definition includes @type:
            if(JsonLdUtils::containsOrEquals(definition[JsonLdConsts::CONTAINER], JsonLdConsts::TYPE)){
                // 19.4.1)
                // If type mapping in definition is undefined, set it to @id.
                // todo this is a place where termdef having member vars like 'containermapping'
                //  and 'typemapping' would come in handy
                if(!definition.contains(JsonLdConsts::TYPE))
                    definition[JsonLdConsts::TYPE] = JsonLdConsts::ID;

                // 19.4.2)
                // If type mapping in definition is neither @id nor @vocab, an invalid type
                // mapping error has been detected and processing is aborted.
                if (definition[JsonLdConsts::TYPE] != JsonLdConsts::ID &&
                    definition[JsonLdConsts::TYPE] != JsonLdConsts::VOCAB) {
                    throw JsonLdError(JsonLdError::InvalidTypeMapping,"");
                }
            }
        }

        // 20)
        // If value contains the entry @index:
        if (value.contains(JsonLdConsts::INDEX)) {

            // 20.1)
            // If processing mode is json-ld-1.0 or container mapping does not include
            // @index, an invalid term definition has been detected and processing is aborted.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0) ||
                !JsonLdUtils::containsOrEquals(definition[JsonLdConsts::CONTAINER], JsonLdConsts::INDEX)) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }

            // 20.2)
            // Initialize index to the value associated with the @index entry. If the result of
            // IRI expanding that value is not an IRI, an invalid term definition has been detected
            // and processing is aborted.
            auto index = value.at(JsonLdConsts::INDEX);

            if (!index.is_string()) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }

            std::string indexStr = index.get<std::string>();

            std::string expandedIndex = expandIri(activeContext, indexStr, false, true, localContext, defined);

            if (!JsonLdUtils::isAbsoluteIri(expandedIndex)) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }

            // 20.3)
            // Set the index mapping of definition to index
            definition[JsonLdConsts::INDEX] = indexStr;
        }

        // 21)
        // If value contains the entry @context:
        if (value.contains(JsonLdConsts::CONTEXT)) {

            // 21.1)
            // If processing mode is json-ld-1.0, an invalid term definition has been detected
            // and processing is aborted.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }

            // 21.2)
            // Initialize context to the value associated with the @context entry, which is treated
            // as a local context.
            auto theContext = value.at(JsonLdConsts::CONTEXT);

            // 21.3)
            // Invoke the Context Processing algorithm using the active context, context as local
            // context, base URL, true for override protected, a copy of remote contexts, and false
            // for validate scoped context. If any error is detected, an invalid scoped context
            // error has been detected and processing is aborted.
            try {
                // Note: The result of the Context Processing algorithm is discarded; it is called
                // to detect errors at definition time. If used, the context will be re-processed
                // and applied to the active context as part of expansion or compaction.
                process(activeContext, theContext, baseURL, iremoteContexts, true, true, false);
            } catch (JsonLdError &error) {
                throw JsonLdError(JsonLdError::InvalidScopedContext,error.what());
            }

            // 21.4)
            // Set the local context of definition to context, and base URL to base URL.
            definition[JsonLdConsts::LOCALCONTEXT] = theContext;
            definition[JsonLdConsts::BASEURL] = baseURL;
        }

        // 22)
        // If value contains the entry @language and does not contain the entry @type:
        if (value.contains(JsonLdConsts::LANGUAGE) && !value.contains(JsonLdConsts::TYPE)) {
            // 22.1)
            // Initialize language to the value associated with the @language entry, which MUST
            // be either null or a string. If language is not well-formed according to section
            // 2.2.9 of [BCP47], processors SHOULD issue a warning. Otherwise, an invalid language
            // mapping error has been detected and processing is aborted.
            auto language = value.at(JsonLdConsts::LANGUAGE);

            if (language.is_null() || language.is_string()) {
                // todo: need to check if language is not well-formed, and if so, emit a warning
                // 22.2)
                // Set the language mapping of definition to language.
                definition[JsonLdConsts::LANGUAGE] = language;
            } else {
                throw JsonLdError(JsonLdError::InvalidLanguageMapping,
                                  "@language must be a string or null");
            }
        }

        // 23.
        // If value contains the entry @direction and does not contain the entry @type:
        if (value.contains(JsonLdConsts::DIRECTION) && !value.contains(JsonLdConsts::TYPE)) {

            // 23.1)
            // Initialize direction to the value associated with the @direction entry, which
            // MUST be either null, "ltr", or "rtl". Otherwise, an invalid base direction error
            // has been detected and processing is aborted.
            auto direction = value.at(JsonLdConsts::DIRECTION);

            if (direction.is_null()) {
                definition[JsonLdConsts::DIRECTION] = direction;
            } else if (direction.is_string()) {

                std::string directionStr = direction.get<std::string>();

                if (directionStr == "ltr" || directionStr == "rtl") {
                    // 23.2)
                    // Set the direction mapping of definition to direction.
                    definition[JsonLdConsts::DIRECTION] = directionStr;
                } else {
                    throw JsonLdError(JsonLdError::InvalidBaseDirection,
                                      R"(@direction must be either "ltr" or "rtl")");
                }
            } else {
                throw JsonLdError(JsonLdError::InvalidBaseDirection,
                                  R"(@direction must be either null, "ltr" or "rtl")");
            }
        }

        // 24)
        // If value contains the entry @nest:
        if (value.contains(JsonLdConsts::NEST)) {

            // 24.1)
            // If processing mode is json-ld-1.0, an invalid term definition has been detected
            // and processing is aborted.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0)) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }

            // 24.2)
            // Initialize nest value in definition to the value associated with the
            // @nest entry, which MUST be a string and MUST NOT be a keyword other
            // than @nest. Otherwise, an invalid @nest value error has been detected
            // and processing is aborted.
            auto nest = value.at(JsonLdConsts::NEST);

            if (!nest.is_string()) {
                throw JsonLdError(JsonLdError::InvalidNestValue,"");
            }

            std::string nestStr = nest.get<std::string>();

            if (JsonLdUtils::isKeyword(nestStr) && nestStr != JsonLdConsts::NEST) {
                throw JsonLdError(JsonLdError::InvalidNestValue,"");
            }
            definition[JsonLdConsts::NEST] = nestStr;
        }

        // 25)
        // If value contains the entry @prefix:
        if (value.contains(JsonLdConsts::PREFIX)) {

            // 25.1)
            // If processing mode is json-ld-1.0, or if term contains a colon (:) or
            // slash (/), an invalid term definition has been detected and processing
            // is aborted.
            if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0) ||
                term.find(':') != std::string::npos ||
                term.find('/') != std::string::npos) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition);
            }

            // 25.2)
            // Set the prefix flag to the value associated with the @prefix entry, which
            // MUST be a boolean. Otherwise, an invalid @prefix value error has been detected
            // and processing is aborted.
            auto prefix = value.at(JsonLdConsts::PREFIX);

            if(!prefix.is_boolean())
                throw JsonLdError(JsonLdError::InvalidPrefixValue,"");

            definition[JsonLdConsts::IS_PREFIX_FLAG] = prefix;

            // 25.3)
            // If the prefix flag of definition is set to true, and its IRI mapping is a
            // keyword, an invalid term definition has been detected and processing is aborted.
            if (definition[JsonLdConsts::IS_PREFIX_FLAG] && JsonLdUtils::isKeyword(definition[JsonLdConsts::ID].get<std::string>())) {
                throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
            }
        }

        // 26)
        // If value contains any entry other than @id, @reverse, @container, @context,
        // @direction, @index, @language, @nest, @prefix, @protected, or @type, an invalid
        // term definition error has been detected and processing is aborted.
        std::set<std::string> validKeywords {
                JsonLdConsts::ID,JsonLdConsts::REVERSE,JsonLdConsts::CONTAINER,JsonLdConsts::CONTEXT,
                JsonLdConsts::DIRECTION,JsonLdConsts::INDEX,JsonLdConsts::LANGUAGE,JsonLdConsts::NEST,
                JsonLdConsts::PREFIX,JsonLdConsts::PROTECTED,JsonLdConsts::TYPE
        };
        for (auto& el : value.items()) {
            if(validKeywords.find(el.key()) == validKeywords.end())
                throw JsonLdError(JsonLdError::InvalidTermDefinition,el.key() + " not in list of valid keywords");
        }

        // 27)
        // If override protected is false and previous definition exists and is protected;
        if(!ioverrideProtected &&
           !previousDefinition.is_null() &&
           previousDefinition.contains(JsonLdConsts::IS_PROTECTED_FLAG) &&
           previousDefinition[JsonLdConsts::IS_PROTECTED_FLAG]) {
            // 27.1)
            // If definition is not the same as previous definition (other than the value
            // of protected), a protected term redefinition error has been detected, and
            // processing is aborted.
            json copyOfDefinition = definition;
            json copyOfPrevious = previousDefinition;
            copyOfDefinition.erase(JsonLdConsts::IS_PROTECTED_FLAG);
            copyOfPrevious.erase(JsonLdConsts::IS_PROTECTED_FLAG);
            if(copyOfDefinition != copyOfPrevious)
                throw JsonLdError(JsonLdError::ProtectedTermRedefinition);

            // 27.2)
            // Set definition to previous definition to retain the value of protected.
            definition = previousDefinition;
        }

        // 28)
        activeContext.termDefinitions[term] = definition;
        defined[term] = true;

    }

/**
  * IRI Expansion Algorithm
  *
  * https://www.w3.org/TR/json-ld11-api/#iri-expansion
  *
  * Notes:
  * If context is_null(), then we don't bother creating term definitions
  * We don't want to bother with a nullable 'defined' so it can just be empty if context is_null()
  *
  * @param value the Iri to expand
  * @param relative flag
  * @param vocab flag
  * @param localContext the local context
  * @param defined map of defined values
  * @return the expanded Iri
  * @throws JsonLdError
  */
    std::string expandIri(Context & activeContext,
                          std::string value, bool relative, bool vocab,
                          const json& localContext, std::map<std::string, bool> & defined) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the IRI expansion algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#iri-expansion

        // 1)
        // If value is a keyword or null, return value as is.
        if (JsonLdUtils::isKeyword(value)) {
            return value;
        }

        // 2)
        //  If value has the form of a keyword (i.e., it matches the ABNF rule "@"1*ALPHA
        //  from [RFC5234]), a processor SHOULD generate a warning and return null.
        if (JsonLdUtils::isKeywordForm(value)) {
            // todo: emit a warning
            return "";
        }

        // 3)
        // If local context is not null, it contains an entry with a key that equals value, and
        // the value of the entry for value in defined is not true, invoke the Create Term
        // Definition algorithm, passing active context, local context, value as term, and
        // defined. This will ensure that a term definition is created for value in active
        // context during Context Processing.
        if (!localContext.is_null() && localContext.contains(value)) {
            const auto& v = localContext.at(value);
            if(v.is_string() &&
               defined.find(v.get<std::string>()) != defined.end() &&
               !defined.at(v.get<std::string>())) {
                createTermDefinition(activeContext, localContext, value, defined);
            }
        }

        // 4)
        // If active context has a term definition for value, and the associated IRI mapping
        // is a keyword, return that keyword.
        if (activeContext.termDefinitions.contains(value)) {
            auto td = activeContext.termDefinitions.at(value);
            if (!td.is_null() &&
                td.contains(JsonLdConsts::ID) &&
                JsonLdUtils::isKeyword(td.at(JsonLdConsts::ID).get<std::string>()))
                return td.at(JsonLdConsts::ID).get<std::string>();
        }

        // 5)
        // If vocab is true and the active context has a term definition for value, return the
        // associated IRI mapping.
        if (vocab && activeContext.termDefinitions.contains(value)) {
            auto td = activeContext.termDefinitions.at(value);
            if (!td.is_null() &&
                td.contains(JsonLdConsts::ID))
                return td.at(JsonLdConsts::ID).get<std::string>();
            else
                return ""; // todo: this return isn't in the spec, but expand_t0032 test fails without it
        }

        // 6)
        // If value contains a colon (:) anywhere after the first character, it is either an IRI, a
        // compact IRI, or a blank node identifier:
        auto colIndex = value.find(':', 1);
        if (colIndex != std::string::npos) {
            // 6.1)
            // Split value into a prefix and suffix at the first occurrence of a colon (:).
            std::string prefix(value, 0, colIndex);
            std::string suffix(value, colIndex+1);

            // 6.2)
            // If prefix is underscore (_) or suffix begins with double-forward-slash (//), return
            // value as it is already an IRI or a blank node identifier.
            if (prefix == "_" || suffix.find("//") == 0) {
                return value;
            }

            // 6.3)
            // If local context is not null, it contains a prefix entry, and the value of the prefix
            // entry in defined is not true, invoke the Create Term Definition algorithm, passing
            // active context, local context, prefix as term, and defined. This will ensure that a
            // term definition is created for prefix in active context during Context Processing.
            if (!localContext.is_null() && localContext.contains(prefix)
                && (defined.find(prefix) == defined.end() || !defined.at(prefix))) {
                createTermDefinition(activeContext, localContext, prefix, defined);
            }

            // 6.4)
            // If active context contains a term definition for prefix having a non-null IRI mapping
            // and the prefix flag of the term definition is true, return the result of concatenating
            // the IRI mapping associated with prefix and suffix.
            if (activeContext.termDefinitions.contains(prefix)) {
                auto prefixDef = activeContext.termDefinitions.at(prefix);
                if (prefixDef.contains(JsonLdConsts::ID) &&
                    prefixDef.contains(JsonLdConsts::IS_PREFIX_FLAG) &&
                    prefixDef.at(JsonLdConsts::IS_PREFIX_FLAG)) {
                    return prefixDef.at(JsonLdConsts::ID).get<std::string>() + suffix;
                }
            }

            // 6.5)
            // If value has the form of an IRI, return value.
            if(Uri::isAbsolute(value))
                return value;
        }

        // 7)
        // If vocab is true, and active context has a vocabulary mapping, return the result of
        // concatenating the vocabulary mapping with value.
        if (vocab && !activeContext.getVocabularyMapping().empty()) {
            return activeContext.getVocabularyMapping() + value;
        }

            // 8)
            // Otherwise, if document relative is true, set value to the result of resolving value
            // against the base IRI from active context. Only the basic algorithm in section 5.2
            // of [RFC3986] is used; neither Syntax-Based Normalization nor Scheme-Based Normalization
            // are performed. Characters additionally allowed in IRI references are treated in the
            // same way that unreserved characters are treated in URI references, per section 6.5
            // of [RFC3987].
        else if (relative) {
            if(!activeContext.getBaseIri().empty())
                return JsonLdUrl::resolve(&(activeContext.getBaseIri()), &value);
            else
                return JsonLdUrl::resolve(nullptr, &value);
        }

        // 9)
        // Return value as is.
        return value;
    }


    Context process(
            const Context & activeContext,
            const nlohmann::json & ilocalContext,
            const std::string &baseURL,
            std::vector<std::string> remoteContexts,
            bool ioverrideProtected,
            bool ipropagate,
            bool ivalidateScopedContext) {

        // Comments in this function are labeled with numbers that correspond to sections
        // from the description of the context processing algorithm.
        // See: https://www.w3.org/TR/json-ld11-api/#context-processing-algorithm

        std::string originalBaseURL = baseURL;
        // todo: get rid of or move these to context?
        bool overrideProtected = ioverrideProtected;
        bool propagate = ipropagate;
        bool validateScopedContext = ivalidateScopedContext;

        // 1)
        // Initialize result to the result of cloning active context, with inverse context set to null.
        Context result = activeContext;
        result.inverseContext = nullptr;

        // 2)
        // If local context is an object containing the member @propagate, its value MUST be
        // boolean true or false, set propagate to that value. Note: Error handling is performed
        // in step 5.11
        if (ilocalContext.is_object()) {
            if (ilocalContext.contains(JsonLdConsts::PROPAGATE)) {
                if(ilocalContext.at(JsonLdConsts::PROPAGATE).is_boolean())
                    propagate = ilocalContext.at(JsonLdConsts::PROPAGATE).get<bool>();
            }
        }

        // 3)
        // If propagate is false, and result does not have a previous context, set previous
        // context in result to active context.
        if (!propagate && result.previousContext == nullptr) {
            result.previousContext = std::make_shared<Context>(activeContext);
        }

        // 4)
        // If local context is not an array, set local context to an array containing only
        // local context.
        json localContext = json::array();
        if (!ilocalContext.is_array())
            localContext = json::array({ilocalContext});
        else
            localContext.insert(localContext.end(), ilocalContext.begin(), ilocalContext.end());

        // 5)
        // For each item context in local context:
        for (auto context : localContext) {
            // 5.1)
            // If context is null:
            if (context.is_null()) {
                // 5.1.1)
                // If override protected is false and active context contains any protected
                // term definitions, an invalid context nullification has been detected and
                // processing is aborted.
                if (!overrideProtected) {
                    // search for any term definitions that have a protected term
                    for (auto term : activeContext.termDefinitions) {
                        if (term.contains(JsonLdConsts::IS_PROTECTED_FLAG) &&
                            term[JsonLdConsts::IS_PROTECTED_FLAG])
                            throw JsonLdError(JsonLdError::InvalidContextNullification);
                    }
                }

                // 5.1.2)
                // Initialize result as a newly-initialized active context, setting both base IRI
                // and original base URL to the value of original base URL in active context,
                // and, if propagate is false, previous context in result to the previous value
                // of result.
                Context c(result.getOptions());
                c.setBaseIri(originalBaseURL);
                c.setOriginalBaseUrl(originalBaseURL);
                if (!propagate)
                    c.previousContext = std::make_shared<Context>(result);
                result = c;

                // 5.1.3)
                // Continue with the next context.
                continue;
            }
                // 5.2)
                // If context is a string,
            else if (context.is_string()) {
                // 5.2.1)
                // Initialize context to the result of resolving context against base URL. If base
                // URL is not a valid IRI, then context MUST be a valid IRI, otherwise a loading
                // document failed error has been detected and processing is aborted.
                std::string contextUri = context.get<std::string>();

                if (!baseURL.empty()) {
                    contextUri = JsonLdUrl::resolve(&baseURL, &contextUri);
                } else if (!JsonLdUtils::isAbsoluteIri(contextUri)) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, contextUri);
                }

                if (!JsonLdUtils::isAbsoluteIri(contextUri)) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, contextUri);
                }

                // 5.2.2)
                // If validate scoped context is false, and remote contexts already includes
                // context do not process context further and continue to any next context in local context.
                if (!validateScopedContext &&
                    std::find(remoteContexts.cbegin(), remoteContexts.cend(), contextUri) != remoteContexts.cend()) {
                    continue;
                }

                // 5.2.3)
                // If the number of entries in the remote contexts array exceeds a processor defined
                // limit, a context overflow error has been detected and processing is aborted;
                // otherwise, add context to remote contexts.
                if (remoteContexts.size() > MAX_REMOTE_CONTEXTS) {
                    throw JsonLdError(JsonLdError::ContextOverflow,
                                      "Too many contexts [>" + std::to_string(MAX_REMOTE_CONTEXTS) + "].");
                }

                remoteContexts.push_back(contextUri);

                // 5.2.4)
                // If context was previously dereferenced, then the processor MUST NOT do a
                // further dereference, and context is set to the previously established
                // internal representation: set context document to the previously dereferenced
                // document, and set loaded context to the value of the @context entry from the
                // document in context document.
                // todo: ...

                // 5.2.5)
                // Otherwise, set context document to the RemoteDocument obtained by dereferencing
                // context using the LoadDocumentCallback, passing context for url, and
                // http://www.w3.org/ns/json-ld#context for profile and for requestProfile.

                // todo, fix indentation here...
                // 5.2.5.1)
                // If context cannot be dereferenced, or the document from context document cannot
                // be transformed into the internal representation , a loading remote context
                // failed error has been detected and processing is aborted.
                if (activeContext.getOptions().getDocumentLoader() == nullptr) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "DocumentLoader is null");
                }
                std::unique_ptr<RemoteDocument> rd;
                try {
                    rd = activeContext.getOptions().getDocumentLoader()->loadDocument(contextUri);
                    if (rd == nullptr)
                        throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "Document is null");
                }
                catch (JsonLdError &error) {
                    std::string msg = error.what();
                    if (msg.find(JsonLdError::LoadingDocumentFailed) != std::string::npos) {
                        throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, msg);
                    }
                }

                const json &document = rd->getJSONContent();
                if (!document.is_object()) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "document is not a json document");
                }

                // 5.2.5.2)
                // If the document has no top-level map with an @context entry, an invalid remote
                // context has been detected and processing is aborted.
                if (!document.contains(JsonLdConsts::CONTEXT)) {
                    throw JsonLdError(JsonLdError::InvalidRemoteContext, "document is missing @context entry");
                }

                // 5.2.5.3)
                // Set loaded context to the value of that entry.
                const json &loadedContext = document[JsonLdConsts::CONTEXT];

                // 5.2.6)
                // Set result to the result of recursively calling this algorithm, passing
                // result for active context, loaded context for local context, the documentUrl
                // of context document for base URL, a copy of remote contexts, and validate
                // scoped context.
                result = process(result, loadedContext, rd->getDocumentUrl(), remoteContexts, overrideProtected, propagate,
                                         validateScopedContext);

                // 5.2.7)
                // Continue with the next context.
                continue;
            }
                // 5.3)
                // If context is not a map, an invalid local context error has been detected
                // and processing is aborted.
            else if (!(context.is_object())) {
                if (context.is_string())
                    throw JsonLdError(JsonLdError::InvalidLocalContext, context);
                else
                    throw JsonLdError(JsonLdError::InvalidLocalContext);
            }

            // 5.4)
            // Otherwise, context is a context definition.

            // 5.5)
            // If context has an @version entry:
            if (context.contains(JsonLdConsts::VERSION)) {
                auto value = context.at(JsonLdConsts::VERSION);
                std::string str;
                if (value.is_string())
                    str = value.get<std::string>();
                else
                    // todo: need a better conversion operation
                    // str = std::to_string(value.get<float>());
                    str = "1.1";

                // 5.5.1)
                // If the associated value is not 1.1, an invalid @version value has been
                // detected, and processing is aborted.
                if (str != "1.1")
                    throw JsonLdError(JsonLdError::InvalidVersionValue);

                // 5.5.2)
                // If processing mode is set to json-ld-1.0, a processing mode conflict error
                // has been detected and processing is aborted.
                if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0))
                    throw JsonLdError(JsonLdError::ProcessingModeConflict);
            }

            // 5.6)
            // If context has an @import entry:
            if (context.contains(JsonLdConsts::IMPORT)) {
                // 5.6.1)
                // If processing mode is json-ld-1.0, an invalid context entry error has
                // been detected and processing is aborted.
                if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0))
                    throw JsonLdError(JsonLdError::InvalidContextEntry);

                auto value = context.at(JsonLdConsts::IMPORT);

                // 5.6.2)
                // Otherwise, if the value of @import is not a string, an invalid @import value
                // error has been detected and processing is aborted.
                if (!value.is_string())
                    throw JsonLdError(JsonLdError::InvalidImportValue);

                // 5.6.3)
                // Initialize import to the result of resolving the value of @import
                // against base URL.
                std::string importStr = value.get<std::string>();
                std::string importUri = JsonLdUrl::resolve(&baseURL, &importStr);

                // 5.6.4)
                // Dereference import using the LoadDocumentCallback, passing import for url, and
                // http://www.w3.org/ns/json-ld#context for profile and for requestProfile.
                if (activeContext.getOptions().getDocumentLoader() == nullptr) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "DocumentLoader is null");
                }
                std::unique_ptr<RemoteDocument> rd;
                try {
                    rd = activeContext.getOptions().getDocumentLoader()->loadDocument(importUri);
                    if (rd == nullptr)
                        throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "Document is null");
                }
                catch (JsonLdError &error) {
                    std::string msg = error.what();
                    if (msg.find(JsonLdError::LoadingDocumentFailed) != std::string::npos) {
                        throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, msg);
                    }
                }
                // 5.6.5)
                // If import cannot be dereferenced, or cannot be transformed into the internal representation, a
                // loading remote context failed error has been detected and processing is aborted.
                const json &document = rd->getJSONContent();
                if (!document.is_object()) {
                    throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "document is not a json document");
                }

                // 5.6.6)
                // If the dereferenced document has no top-level map with an @context entry, or
                // if the value of @context is not a context definition (i.e., it is not an map),
                // an invalid remote context has been detected and processing is aborted; otherwise,
                // set import context to the value of that entry.
                if (!document.contains(JsonLdConsts::CONTEXT)) {
                    throw JsonLdError(JsonLdError::InvalidRemoteContext, "document is missing @context entry");
                }
                json importContext = document[JsonLdConsts::CONTEXT];
                if (!importContext.is_object()) {
                    throw JsonLdError(JsonLdError::InvalidRemoteContext, "@context is not a context definition");
                }

                // 5.6.7)
                // If import context has a @import entry, an invalid context entry error
                // has been detected and processing is aborted.
                if (importContext.contains(JsonLdConsts::IMPORT))
                    throw JsonLdError(JsonLdError::InvalidContextEntry, "@import context has an @import entry");

                // 5.6.8)
                // Set context to the result of merging context into import context, replacing
                // common entries with those from context.
                importContext.update(context);
                context = importContext;

            }

            // 5.7)
            // If context has an @base entry and remote contexts is empty, i.e., the currently
            // being processed context is not a remote context:
            if (remoteContexts.empty() && context.contains(JsonLdConsts::BASE)) {
                // 5.7.1)
                // Initialize value to the value associated with the @base entry.
                auto value = context.at(JsonLdConsts::BASE);
                // 5.7.2)
                // If value is null, remove the base IRI of result.
                if (value.is_null()) {
                    result.setBaseIri("");
                } else if (value.is_string()) {
                    // 5.7.3)
                    // Otherwise, if value is an IRI, the base IRI of result is set to value.
                    if (JsonLdUtils::isAbsoluteIri(value.get<std::string>())) {
                        result.setBaseIri(value.get<std::string>());
                    } else if (JsonLdUtils::isRelativeIri(value.get<std::string>())){
                        // 5.7.4)
                        // Otherwise, if value is a relative IRI reference and the base IRI of
                        // result is not null, set the base IRI of result to the result of resolving
                        // value against the current base IRI of result.
                        if (result.getBaseIri().empty())
                            throw JsonLdError(JsonLdError::InvalidBaseIri, "baseUri is empty");
                        std::string tmpIri = value.get<std::string>();
                        std::string resolvedIri = JsonLdUrl::resolve(&result.getBaseIri(), &tmpIri);
                        result.setBaseIri(resolvedIri);
                    }
                    else {
                        // 5.7.5)
                        // Otherwise, an invalid base IRI error has been detected and processing is aborted.
                        throw JsonLdError(JsonLdError::InvalidBaseIri, "@base must be a string");
                    }
                } else {
                    // 5.7.5)
                    // Otherwise, an invalid base IRI error has been detected and processing is aborted.
                    throw JsonLdError(JsonLdError::InvalidBaseIri, "@base must be a string");
                }
            }

            // 5.8)
            // If context has an @vocab entry:
            if (context.contains(JsonLdConsts::VOCAB)) {
                // 5.8.1)
                // Initialize value to the value associated with the @vocab entry.
                auto value = context.at(JsonLdConsts::VOCAB);
                // 5.8.2)
                // If value is null, remove any vocabulary mapping from result.
                if (value.is_null()) {
                    result.setVocabularyMapping("");
                } else if (value.is_string()) {
                    // 5.8.3)
                    // Otherwise, if value is an IRI or blank node identifier, the vocabulary
                    // mapping of result is set to the result of IRI expanding value using
                    // true for document relative. If it is not an IRI, or a blank node
                    // identifier, an invalid vocab mapping error has been detected and
                    // processing is aborted.
                    if (value.get<std::string>().empty() ||
                            BlankNodeNames::hasFormOfBlankNodeName(value.get<std::string>()) || JsonLdUtils::isIri(value.get<std::string>())) {
                        std::string vocabMapping = ContextProcessor::expandIri(result, value.get<std::string>(), true, true);
                        result.setVocabularyMapping(vocabMapping);
                    } else {
                        throw JsonLdError(JsonLdError::InvalidVocabMapping,
                                          "@value must be an absolute IRI");
                    }
                } else {
                    throw JsonLdError(JsonLdError::InvalidVocabMapping,
                                      "@vocab must be a string or null");
                }
            }

            // 5.9)
            // If context has an @language entry:
            if (context.contains(JsonLdConsts::LANGUAGE)) {
                // 5.9.1)
                // Initialize value to the value associated with the @language entry.
                auto value = context.at(JsonLdConsts::LANGUAGE);
                if (value.is_null()) {
                    // 5.9.2)
                    // If value is null, remove any default language from result.
                    result.setDefaultLanguage("");
                } else if (value.is_string()) {
                    // 5.9.3)
                    // Otherwise, if value is a string, the default language of result is set
                    // to value. If it is not a string, an invalid default language error has
                    // been detected and processing is aborted. If value is not well-formed
                    // according to section 2.2.9 of [BCP47], processors SHOULD issue a warning.
                    std::string v = value.get<std::string>();
                    std::transform(v.begin(), v.end(), v.begin(), &::tolower);
                    result.setDefaultLanguage(v);
                    // todo: need to check if language is not well-formed, and if so, emit a warning
                } else {
                    throw JsonLdError(JsonLdError::InvalidDefaultLanguage);
                }
            }

            // 5.10)
            // If context has an @direction entry:
            if (context.contains(JsonLdConsts::DIRECTION)) {
                // 5.10.1)
                // If processing mode is json-ld-1.0, an invalid context entry error has been
                // detected and processing is aborted.
                if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0))
                    throw JsonLdError(JsonLdError::InvalidContextEntry);
                // 5.10.2)
                // Initialize value to the value associated with the @direction entry.
                auto value = context.at(JsonLdConsts::DIRECTION);
                if (value.is_null()) {
                    // 5.10.3)
                    // If value is null, remove any base direction from result.
                    result.setDefaultBaseDirection("null");
                } else if (value.is_string()) {
                    // 5.10.4)
                    // Otherwise, if value is a string, the base direction of result is set
                    // to value. If it is not null, "ltr", or "rtl", an invalid base direction
                    // error has been detected and processing is aborted.
                    std::string directionStr = value.get<std::string>();

                    if (directionStr == "ltr" || directionStr == "rtl") {
                        result.setDefaultBaseDirection(directionStr);
                    } else {
                        throw JsonLdError(JsonLdError::InvalidBaseDirection,
                                          R"(@direction must be either "ltr" or "rtl")");
                    }
                } else {
                    throw JsonLdError(JsonLdError::InvalidBaseDirection,
                                      R"(@direction must be either null, "ltr" or "rtl")");
                }
            }

            // 5.11)
            if (context.contains(JsonLdConsts::PROPAGATE)) {
                // 5.11.1)
                if (activeContext.isProcessingMode(JsonLdConsts::JSON_LD_1_0))
                    throw JsonLdError(JsonLdError::InvalidContextEntry);
                // 5.11.2)
                if (!context.at(JsonLdConsts::PROPAGATE).is_boolean()) {
                    throw JsonLdError(JsonLdError::InvalidPropagateValue,
                                      R"(@propagate must be either true or false)");
                }
            }

            // 5.12)
            // Create a map defined to keep track of whether or not a term has already been
            // defined or is currently being defined during recursion.
            std::map<std::string, bool> defined;

            // 5.13)
            // For each key-value pair in context where key is not @base, @direction, @import,
            // @language, @propagate, @protected, @version, or @vocab, invoke the Create Term
            // Definition algorithm, passing result for active context, context for local
            // context, key, defined, base URL, the value of the @protected entry from
            // context, if any, for protected, override protected, and a copy of remote contexts.
            for (const auto &el : context.items()) {
                const auto &key = el.key();
                if (key == JsonLdConsts::BASE || key == JsonLdConsts::DIRECTION || key == JsonLdConsts::IMPORT ||
                    key == JsonLdConsts::LANGUAGE || key == JsonLdConsts::PROPAGATE || key == JsonLdConsts::PROTECTED ||
                    key == JsonLdConsts::VERSION || key == JsonLdConsts::VOCAB) {
                    continue;
                }
                bool isProtected = false;
                if(context.contains(JsonLdConsts::PROTECTED))
                    isProtected = context[JsonLdConsts::PROTECTED].get<bool>();
                createTermDefinition(result, context, key, defined, baseURL, isProtected, overrideProtected, remoteContexts);
            }

        }

        return result;
    }

}

Context
ContextProcessor::process(
        const Context & activeContext,
        const nlohmann::json & ilocalContext,
        const std::string &baseURL,
        bool ioverrideProtected,
        bool ipropagate,
        bool ivalidateScopedContext) {

    std::vector<std::string> remoteContexts;
    return ::process(activeContext, ilocalContext, baseURL, remoteContexts,
                   ioverrideProtected, ipropagate, ivalidateScopedContext);
}

std::string ContextProcessor::expandIri(Context & activeContext, std::string value, bool relative, bool vocab) {
    // dummy objects
    json j;
    std::map<std::string, bool> m;
    return ::expandIri(activeContext, std::move(value), relative, vocab, j, m);
}

