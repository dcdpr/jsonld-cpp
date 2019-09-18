#include <MacTypes.h>
#include <iostream>
#include "Context.h"
#include "JsonLdUrl.h"
#include "ObjUtils.h"

using nlohmann::json;

void Context::checkEmptyKey(json map) {
    auto it = map.find("");
    if (it != map.end()) {
        // the term MUST NOT be an empty string ("")
        // https://www.w3.org/TR/json-ld/#h3_terms
        throw JsonLdError(JsonLdError::InvalidTermDefinition, "empty key for value");
    }
}

void Context::checkEmptyKey(std::map<std::string, std::string> map) {
    auto it = map.find("");
    if (it != map.end()) {
        // the term MUST NOT be an empty string ("")
        // https://www.w3.org/TR/json-ld/#h3_terms
        throw JsonLdError(JsonLdError::InvalidTermDefinition, "empty key for value");
    }
}

void Context::init() {
    contextMap.insert(std::make_pair(JsonLdConsts::BASE, options.getBase()));
    termDefinitions = ObjUtils::newMap();
}

/**
 * Context Processing Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#context-processing-algorithms
 *
 * @param localContext
 *            The Local Context object.
 * @return The parsed and merged Context.
 * @throws JsonLdError
 *             If there is an error parsing the contexts.
 */
Context Context::parse(json localContext)  {
    return parse(localContext, std::vector<std::string>(), false);
}

/**
 * Context Processing Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#context-processing-algorithms
 *
 * @param localContext
 *            The Local Context object.
 * @param remoteContexts
 *            The list of Strings denoting the remote Context URLs.
 * @return The parsed and merged Context.
 * @throws JsonLdError
 *             If there is an error parsing the contexts.
 */
Context Context::parse(json localContext, std::vector<std::string> remoteContexts)  {
    return parse(localContext, remoteContexts, false);
}

/**
 * Helper method used to work around logic errors related to the recursive
 * nature of the JSONLD-API Context Processing Algorithm.
 *
 * @param localContext
 *            The Local Context object.
 * @param remoteContexts
 *            The list of Strings denoting the remote Context URLs.
 * @param parsingARemoteContext
 *            True if localContext represents a remote context that has been
 *            parsed and sent into this method and false otherwise. This
 *            must be set to know whether to propagate the @code{@base} key
 *            from the context to the result.
 * @return The parsed and merged Context.
 * @throws JsonLdError
 *             If there is an error parsing the contexts.
 */
 Context Context::parse(json localContext, std::vector<std::string> remoteContexts,
                      bool parsingARemoteContext) {

//    if (remoteContexts == null) {
//        remoteContexts = new ArrayList<String>();
//    }
    // 1. Initialize result to the result of cloning active context.
    Context *result = new Context(*this); // todo: needs a smart pointer so we don't leak memory
    // 2)

    // set up an array for the loop in 3)
    json myContext = json::array();
    if (!localContext.is_array()) {
        myContext.push_back(localContext);
    }
    else {
        myContext.insert(myContext.end(), localContext.begin(), localContext.end());
    }

    // 3)
    for (auto context : myContext) {
        // 3.1)
        if (context.is_null()) {
            result = new Context(options); // todo: needs a smart pointer so we don't leak memory
            continue;
        }
//        else if (context instanceof Context) { // todo: will this happen in c++ version?
//            result = ((Context) context).clone();
//        }
        // 3.2)
        else if (context.is_string()) {
            throw JsonLdError(JsonLdError::NotImplemented, "context.is_string() not implemented: ");
//            std::string uri = result->get(JsonLdConsts::BASE);
//            uri = JsonLdUrl.resolve(uri, (String) context);
//            // 3.2.2
//            if (remoteContexts.contains(uri)) {
//                throw new JsonLdError(Error.RECURSIVE_CONTEXT_INCLUSION, uri);
//            }
//            remoteContexts.add(uri);
//
//            // 3.2.3: Dereference context
//            final RemoteDocument rd = this.options.getDocumentLoader().loadDocument(uri);
//            final Object remoteContext = rd.getDocument();
//            if (!(remoteContext instanceof Map) || !((Map<String, Object>) remoteContext)
//                    .containsKey(JsonLdConsts.CONTEXT)) {
//                // If the dereferenced document has no top-level JSON object
//                // with an @context member
//                throw new JsonLdError(Error.INVALID_REMOTE_CONTEXT, context);
//            }
//            final Object tempContext = ((Map<String, Object>) remoteContext)
//                    .get(JsonLdConsts.CONTEXT);
//
//            // 3.2.4
//            result = result.parse(tempContext, remoteContexts, true);
            // 3.2.5
            continue;
        } else if (!(context.is_object())) {
            // 3.3
            throw JsonLdError(JsonLdError::InvalidLocalContext, context);
        }
        checkEmptyKey(context);
        // 3.4
        if (!parsingARemoteContext && context.contains(JsonLdConsts::BASE)) {
            // 3.4.1
            auto value = context.at(JsonLdConsts::BASE);
            // 3.4.2
            if (value.is_null()) {
                result->erase(JsonLdConsts::BASE);
            } else if (value.is_string()) {
                // 3.4.3
                if (JsonLdUtils::isAbsoluteIri(value)) {
                    result->insert(std::make_pair(JsonLdConsts::BASE, value.get<std::string>()));
                } else {
                    // 3.4.4
                    std::string baseUri = result->at(JsonLdConsts::BASE);
                    if (!JsonLdUtils::isAbsoluteIri(baseUri)) {
                        throw JsonLdError(JsonLdError::InvalidBaseIri, baseUri);
                    }
                    std::string tmpIri = value.get<std::string>();
                    result->insert(std::make_pair(JsonLdConsts::BASE, JsonLdUrl::resolve(&baseUri, &tmpIri)));
                }
            } else {
                // 3.4.5
                throw JsonLdError(JsonLdError::InvalidBaseIri, "@base must be a string");
            }
        }

        // 3.5
        if (context.contains(JsonLdConsts::VOCAB)) {
            auto value = context.at(JsonLdConsts::VOCAB);
            if (value.is_null()) {
                result->erase(JsonLdConsts::VOCAB);
            } else if (value.is_string()) {
                if (JsonLdUtils::isAbsoluteIri(value)) {
                    result->insert(std::make_pair(JsonLdConsts::VOCAB, value.get<std::string>()));
                } else {
                    throw JsonLdError(JsonLdError::InvalidVocabMapping,
                                      "@value must be an absolute IRI");
                }
            } else {
                throw JsonLdError(JsonLdError::InvalidVocabMapping,
                                  "@vocab must be a string or null");
            }
        }

        // 3.6
        if (context.contains(JsonLdConsts::LANGUAGE)) {
            auto value = context.at(JsonLdConsts::LANGUAGE);
            if (value.is_null()) {
                result->erase(JsonLdConsts::LANGUAGE);
            } else if (value.is_string()) {
                result->insert(std::make_pair(JsonLdConsts::LANGUAGE, value.get<std::string>())); // todo: value to lowercase
            } else {
                throw JsonLdError(JsonLdError::InvalidDefaultLanguage, value);
            }
        }

        // 3.7
        std::map<std::string, bool> defined; //todo: thrown out?
        for (const auto& el : context.items()) {
            const auto& key = el.key();
            if (key == JsonLdConsts::BASE || key == JsonLdConsts::VOCAB || key == JsonLdConsts::LANGUAGE) {
                continue;
            }
            result->createTermDefinition(context, key, defined);
        }
    }

    std::cout << "before returning from parse" << std::endl;
    result->printInternalMap();
    return *result;
}

std::string Context::getContainer(std::string property) {
//        if (property == null) {
//            return null;
//        }
    if (property == JsonLdConsts::GRAPH) {
        return JsonLdConsts::SET;
    }
    if (property != JsonLdConsts::TYPE && JsonLdUtils::isKeyword(property)) {
        return property;
    }
    auto td = termDefinitions[property];
//        if (td == null) {
//            return null;
//        }
    if(td.empty())
        return "";
    auto c = td[JsonLdConsts::CONTAINER];
    if(!c.is_string()) {
        return "";
    }
    else {
        return c.get<std::string>();
    }
}

/**
  * IRI Expansion Algorithm
  *
  * http://json-ld.org/spec/latest/json-ld-api/#iri-expansion
  *
  * Notes:
  * If context is_null(), then we don't bother creating term definitions
  * We don't want to bother with a nullable 'defined' so it can just be empty if context is_null()
  *
  * @param value
  * @param relative
  * @param vocab
  * @param context
  * @param defined
  * @return
  * @throws JsonLdError
  */
std::string Context::expandIri(
        std::string value, bool relative, bool vocab,
        json context, std::map<std::string, bool> & defined) {
        // 1)
        if (JsonLdUtils::isKeyword(value)) { // todo: also checked for if value was null
            return value;
        }
        // 2)
        if (!context.is_null() && context.contains(value) &&
            defined.find(value) != defined.end() && !defined.at(value) ) {
            createTermDefinition(context, value, defined);
        }
        // 3)
        if (vocab && termDefinitions.find(value) != termDefinitions.end()) {
            auto td = termDefinitions.at(value);
            if (!td.is_null()) {
                if(td.contains(JsonLdConsts::ID))
                    return td.at(JsonLdConsts::ID);
                else
                    return "";
            } else {
                return ""; // todo: was null
            }
        }
        // 4)
        auto colIndex = value.find(':');
        if (colIndex != std::string::npos) {
            // 4.1)
            std::string prefix(value, 0, colIndex);
            std::string suffix(value, colIndex+1);
            // 4.2)
            if (prefix == "_" || suffix.find('/') == 0) {
                return value;
            }
            // 4.3)
            if (!context.is_null() && context.contains(prefix)
                && (defined.find(prefix) == defined.end() || !defined.at(prefix))) {
                createTermDefinition(context, prefix, defined);
            }
            // 4.4)
            if (termDefinitions.find(prefix) != termDefinitions.end()) {
                auto id = termDefinitions.at(prefix).at(JsonLdConsts::ID);
                id = id.get<std::string>() + suffix;
                return id;
            }
            // 4.5)
            return value;
        }
        // 5)
        if (vocab && contextMap.find(JsonLdConsts::VOCAB) != contextMap.end()) {
            return contextMap.at(JsonLdConsts::VOCAB) + value;
        }
        // 6)
        else if (relative) {
            if(this->count(JsonLdConsts::BASE))
                return JsonLdUrl::resolve(&(this->at(JsonLdConsts::BASE)), &value);
            else
                return JsonLdUrl::resolve(nullptr, &value);
        } else if (!context.is_null() && JsonLdUtils::isRelativeIri(value)) {
            throw JsonLdError(JsonLdError::InvalidIriMapping, "not an absolute IRI: " + value);
        }
        // 7)
        return value;
}

std::string Context::expandIri(
        std::string value, bool relative, bool vocab) {
    // dummy objects
    json j;
    std::map<std::string, bool> m;
    return expandIri(value, relative, vocab, j, m);
}

/**
 * Create Term Definition Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#create-term-definition
 * https://w3c.github.io/json-ld-api/#create-term-definition
 *
 * @param result
 * @param context
 * @param key
 * @param defined
 * @throws JsonLdError
 */
void Context::createTermDefinition(json context, std::string term,
                                  std::map<std::string, bool> & defined)  {

    std::cout << "createTermDefinition: " << term << std::endl;

    // 1) has term been defined already?
    if (defined.find(term) != defined.end()) {
        if (defined.at(term)) {
            return;
        }
        throw JsonLdError(JsonLdError::CyclicIriMapping, term);
    }

    // 2) term is bring created, but not yet complete
    defined[term] = false;

    // 4) + 5) check for keyword redefinition

    // if term is a keyword
    if (JsonLdUtils::isKeyword(term)) {
        // if term is not ('@type' and getAllowContainerSetOnType is true)
        if(!(options.getAllowContainerSetOnType() && term == JsonLdConsts::TYPE)) {
            // if context for term contain '@id'
            if( !(context.at(term)).contains(JsonLdConsts::ID)) {
                throw JsonLdError(JsonLdError::KeywordRedefinition, term);
            }
        }
    }

    // 7) remove any previous definition
    if(termDefinitions.count(term) ) {
        termDefinitions.erase(term);
    }

    // 3) get value associated with term
    auto value = context.at(term);

    // 8) If value is null, create a single member whose key is @id and whose value is null.

    // todo: can these ever really be nullptr?

    if (value == nullptr ||
        (value.contains(JsonLdConsts::ID) && value.at(JsonLdConsts::ID) == nullptr)) {
        termDefinitions[term] = nullptr;
        defined[term] = true;
        std::cout << "createTermDefinition: def: null" << std::endl;
        std::cout << "createTermDefinition: " << term << " created." << std::endl;
        return;
    }

    // 9)
    if (value.is_string()) {
        value = ObjUtils::newMap(JsonLdConsts::ID, value);
    }

    // 10)
    if (!(value.is_object())) {
        throw JsonLdError(JsonLdError::InvalidTermDefinition, value);
    }

    // 11) create a new term definition
    auto definition = ObjUtils::newMap();

    // 12) ???

    // 13)
    if (value.contains(JsonLdConsts::TYPE)) {
        auto type = value.at(JsonLdConsts::TYPE);

        // 13.1)
        if (!(type.is_string())) {
            throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
        }
        std::string typeStr = type.get<std::string>();

        // 13.2)
        try {
            typeStr = expandIri(typeStr, false, true, context, defined);
//        } catch (JsonLdError::InvalidIriMapping &error) { // todo, what to do about the type?
//            throw error;
        } catch (JsonLdError &error) {
            if(typeid(error.getType()) == typeid(JsonLdError::InvalidIriMapping)) {
                throw error;
            }
            throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
        }

        // TODO: fix check for absoluteIri (blank nodes shouldn't count, at least not here!)
        // 13.3)
        if (typeStr == JsonLdConsts::ID || typeStr == JsonLdConsts::VOCAB
            || (typeStr.find(JsonLdConsts::BLANK_NODE_PREFIX) != 0
                && JsonLdUtils::isAbsoluteIri(typeStr))) {
            definition[JsonLdConsts::TYPE] = typeStr;
        } else {
            throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
        }
    }

        // 11)
        if (value.contains(JsonLdConsts::REVERSE)) {
            if (value.contains(JsonLdConsts::ID)) {
                throw JsonLdError(JsonLdError::InvalidReverseProperty, value);
            }
            auto reverse = value.at(JsonLdConsts::REVERSE);

            if (!(reverse.is_string())) {
                throw  JsonLdError(JsonLdError::InvalidIriMapping,
                                      "Expected String for @reverse value.");
            }
            std::string reverseStr = reverse.get<std::string>();

            reverseStr = expandIri(reverseStr, false, true, context, defined);
            if (!JsonLdUtils::isAbsoluteIri(reverseStr)) {
                throw JsonLdError(JsonLdError::InvalidIriMapping,
                                      "Non-absolute @reverse IRI: " + reverseStr);
            }
            definition[JsonLdConsts::ID] = reverseStr;
            if (value.contains(JsonLdConsts::CONTAINER)) {
                std::string container = value.at(JsonLdConsts::CONTAINER);
                if (container == JsonLdConsts::SET || container == JsonLdConsts::INDEX) { // todo was container == null
                    definition[JsonLdConsts::CONTAINER] = container;
                } else {
                    throw JsonLdError(JsonLdError::InvalidReverseProperty,
                                          "reverse properties only support set- and index-containers");
                }
            }
            definition[JsonLdConsts::REVERSE] = true;
            termDefinitions[term] = definition;
            defined[term] = true;
            std::cout << "createTermDefinition: def: " << definition << std::endl;
            std::cout << "createTermDefinition: " << term << " created." << std::endl;
            return;
        }

        // 12)
        definition[JsonLdConsts::REVERSE] = false;

        // 13)
        if (value.contains(JsonLdConsts::ID) && term != value.at(JsonLdConsts::ID)) {
            auto id = value.at(JsonLdConsts::ID);
            if (!id.is_string()) {
                throw JsonLdError(JsonLdError::InvalidIriMapping, "expected value of @id to be a string");
            }

            std::string idStr = id.get<std::string>();

            idStr = expandIri(idStr, false, true, context, defined);
            if (JsonLdUtils::isKeyword(idStr) || JsonLdUtils::isAbsoluteIri(idStr)) {
                if (idStr == JsonLdConsts::CONTEXT) {
                    throw JsonLdError(JsonLdError::InvalidKeywordAlias, "cannot alias @context");
                }
                definition[JsonLdConsts::ID] = idStr;
            } else {
                throw JsonLdError(JsonLdError::InvalidIriMapping,
                                  "resulting IRI mapping should be a keyword, absolute IRI or blank node");
            }
        }

        // 14)
        else if (term.find(':') != std::string::npos) { // todo: use isabsoluteiri?
            auto colIndex = term.find(':');
            std::string prefix(term, 0, colIndex);
            std::string suffix(term, colIndex+1);
            if (context.contains(prefix)) {
                createTermDefinition(context, prefix, defined);
            }
            if (termDefinitions.find(prefix) != termDefinitions.end()) {
                auto id = termDefinitions.at(prefix).at(JsonLdConsts::ID);
                //id += suffix;
                id = id.get<std::string>() + suffix; // todo: can we assume string?
                definition[JsonLdConsts::ID] = id;
            } else {
                definition[JsonLdConsts::ID] = term;
            }
            // 15)
        } else if (contextMap.find(JsonLdConsts::VOCAB) != contextMap.end()) {
            definition[JsonLdConsts::ID] = contextMap.at(JsonLdConsts::VOCAB) + term;
        } else if (term != JsonLdConsts::TYPE) {
            throw JsonLdError(JsonLdError::InvalidIriMapping,
                                  "relative term definition without vocab mapping");
        }

        // 16)
        if (value.contains(JsonLdConsts::CONTAINER)) {
            std::string container = value.at(JsonLdConsts::CONTAINER);
            if(container != JsonLdConsts::LIST &&
               container != JsonLdConsts::SET &&
               container != JsonLdConsts::INDEX &&
               container != JsonLdConsts::LANGUAGE) {
                throw JsonLdError(JsonLdError::InvalidContainerMapping,
                                  "@container must be either @list, @set, @index, or @language");
            }
            definition[JsonLdConsts::CONTAINER] = container;
            if (term == JsonLdConsts::TYPE) {
                definition[JsonLdConsts::ID] = "type";
            }
        }

        // 17)
        if (value.contains(JsonLdConsts::LANGUAGE) && !value.contains(JsonLdConsts::TYPE)) {
            auto language = value.at(JsonLdConsts::LANGUAGE);
            if (language.is_null() || language.is_string()) {
                definition[JsonLdConsts::LANGUAGE] = language; // todo: tolowercase or null?
            } else {
                throw JsonLdError(JsonLdError::InvalidLanguageMapping,
                                      "@language must be a string or null");
            }
        }

        // 18)
        termDefinitions[term] = definition;
        defined[term] = true;

    std::cout << "createTermDefinition: def: " << definition << std::endl;
    std::cout << "createTermDefinition: " << term << " created." << std::endl;

}

std::string & Context::at(const std::string &s) {
    return contextMap.at(s);
}

size_t Context::erase(const std::string &key) {
    return contextMap.erase(key);
}

std::pair<Context::StringMap::iterator,bool> Context::insert( const StringMap::value_type& value ) {
    // unlike a normal c++ map, which does not insert a value if the key is already present,
    // we DO want to replace, so we have to erase first
    if(contextMap.find(value.first) != contextMap.end()) {
        contextMap.erase(value.first);
    }
    return contextMap.insert(value);
}

size_t Context::count(const std::string &key) const {
    return contextMap.count(key);
}

bool Context::isReverseProperty(const std::string &property) {
    if(!termDefinitions.count(property)) {
        return false;
    }
    auto td = termDefinitions.at(property);
    if (td.is_null()) {
        return false;
    }
    std::cout << "isReverseProperty: td: " << td << std::endl;
    return td.contains(JsonLdConsts::REVERSE) && td.at(JsonLdConsts::REVERSE);
}

nlohmann::json Context::getTermDefinition(std::string key) {
    if(termDefinitions.count(key)) {
        return termDefinitions.at(key);
    }
    else
        return json::object();
}


json Context::expandValue(std::string activeProperty, json value)  {
    auto rval = ObjUtils::newMap();
    json td = getTermDefinition(activeProperty);
    // 1)
    if (!td.is_null() && td.contains(JsonLdConsts::TYPE) && td.at(JsonLdConsts::TYPE) == JsonLdConsts::ID) {
        // TODO: i'm pretty sure value should be a string if the @type is @id
        rval[JsonLdConsts::ID] = expandIri(value.get<std::string>(), true, false);
        return rval;
    }
    // 2)
    if (!td.is_null() && td.contains(JsonLdConsts::TYPE) && td.at(JsonLdConsts::TYPE) == JsonLdConsts::VOCAB) {
        // TODO: same as above
        rval[JsonLdConsts::ID] = expandIri(value.get<std::string>(), true, true);
        return rval;
    }
    // 3)
    rval[JsonLdConsts::VALUE] = value;
    // 4)
    if (!td.is_null() && td.contains(JsonLdConsts::TYPE)) {
        rval[JsonLdConsts::TYPE] = td.at(JsonLdConsts::TYPE);
    }
        // 5)
    else if (value.is_string()) {
        // 5.1)
        if (!td.is_null() && td.contains(JsonLdConsts::LANGUAGE)) {
            json lang = td.at(JsonLdConsts::LANGUAGE);
            if (!lang.is_null()) {
                rval[JsonLdConsts::LANGUAGE] = lang.get<std::string>();
            }
        }
            // 5.2)
        else if (contextMap.count(JsonLdConsts::LANGUAGE)) {
            rval[JsonLdConsts::LANGUAGE] = contextMap.at(JsonLdConsts::LANGUAGE);
        }
    }
    return rval;
}

void Context::printInternalMap() {

    std::cout << "Size of context map: " << contextMap.size() << std::endl;
    int i=0;
    for(const auto& el : contextMap) {
        std::cout << i++ << ": " << el.first << " -> " << el.second << std::endl;
    }
}

