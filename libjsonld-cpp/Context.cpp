#include "Context.h"
#include "JsonLdUrl.h"
#include "ObjUtils.h"
#include "Uri.h"
#include "BlankNode.h"
#include <iostream>
#include <utility>
#include <set>

using nlohmann::json;

namespace {

    // 1.1 spec provides for returning error if too many remote contexts are loaded
    static const int MAX_REMOTE_CONTEXTS = 256;

}

void Context::checkEmptyKey(const json& map) {
    if(map.count("")) {
        // the term MUST NOT be an empty string ("")
        // https://www.w3.org/TR/json-ld/#h3_terms
        throw JsonLdError(JsonLdError::InvalidTermDefinition, "empty key for value");
    }
}

void Context::checkEmptyKey(const std::map<std::string, std::string>& map) {
    if(map.count("")) {
        // the term MUST NOT be an empty string ("")
        // https://www.w3.org/TR/json-ld/#h3_terms
        throw JsonLdError(JsonLdError::InvalidTermDefinition, "empty key for value");
    }
}

void Context::init() {
    contextMap.insert(std::make_pair(JsonLdConsts::BASE, options.getBase()));
    termDefinitions = ObjUtils::newMap();
    inverseContext = nullptr;
    previousContext = nullptr;
    overrideProtected = false;
    propagate = true;
    validateScopedContext = true;
    defaultBaseDirection = "null";
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
Context Context::parse(const nlohmann::json & localContext, const std::string & baseURL) {
    return parse(localContext, baseURL, remoteContexts);
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
//Context Context::parse(const json & localContext, std::vector<std::string> & remoteContexts)  {
//    return parse(localContext, remoteContexts, false);
//}

/**
 * Helper method used to work around logic errors related to the recursive
 * nature of the JSONLD-API Context Processing Algorithm.
 *
 * @param localContext
 *            The Local Context object.
 * @param iremoteContexts
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
Context Context::parse(const json & ilocalContext, const std::string & baseURL,
                         std::vector<std::string> & iremoteContexts,
                         bool ioverrideProtected,
                         bool ipropagate,
                         bool ivalidateScopedContext)  {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the context processing algorithm.
    // See: https://w3c.github.io/json-ld-api/#context-processing-algorithm

    // todo: remove
    std::cout << "... 1.1 Context::parse()\n";

    originalBaseURL = baseURL;
    remoteContexts = iremoteContexts;
    overrideProtected = ioverrideProtected;
    propagate = ipropagate;
    validateScopedContext = ivalidateScopedContext;

    // 1)
    // Initialize result to the result of cloning active context, with inverse context set to null.
    Context result = *this;
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
        result.previousContext = this;
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
                for (auto term : termDefinitions) {
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
            Context c(options);
            c.baseIRI = originalBaseURL;
            c.originalBaseURL = originalBaseURL;
            if (!propagate)
                c.previousContext = &result;
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

            if (!baseURL.empty()) {  // todo: was null
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
                std::find(remoteContexts.begin(), remoteContexts.end(), contextUri) != remoteContexts.end()) {
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

            // 5.2.5)
            // Otherwise, set context document to the RemoteDocument obtained by dereferencing
            // context using the LoadDocumentCallback, passing context for url, and
            // http://www.w3.org/ns/json-ld#context for profile and for requestProfile.
            if (options.getDocumentLoader() == nullptr) {
                throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "DocumentLoader is null");
            }

            // 5.2.5.1)
            // If context cannot be dereferenced, or the document from context document cannot
            // be transformed into the internal representation , a loading remote context
            // failed error has been detected and processing is aborted.
            std::unique_ptr<RemoteDocument> rd;
            try {
                rd = options.getDocumentLoader()->loadDocument(contextUri);
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
            result = result.parse(loadedContext, rd->getDocumentUrl(), remoteContexts, overrideProtected, propagate,
                                    validateScopedContext);

            // 5.2.7)
            // Continue with the next context.
            continue;

        } else if (!(context.is_object())) {
            // 5.3)
            // If context is not a map, an invalid local context error has been detected
            // and processing is aborted.
            if (context.is_string())
                throw JsonLdError(JsonLdError::InvalidLocalContext, context);
            else
                throw JsonLdError(JsonLdError::InvalidLocalContext);
        }
        checkEmptyKey(context);

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
                //todo need a better conversion operation
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
            if (isProcessingMode(JsonLdOptions::JSON_LD_1_0))
                throw JsonLdError(JsonLdError::ProcessingModeConflict);
        }

        // 5.6)
        // If context has an @import entry:
        if (context.contains(JsonLdConsts::IMPORT)) {
            // 5.6.1)
            // If processing mode is json-ld-1.0, an invalid context entry error has
            // been detected and processing is aborted.
            if (isProcessingMode(JsonLdOptions::JSON_LD_1_0))
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
            if (options.getDocumentLoader() == nullptr) {
                throw JsonLdError(JsonLdError::LoadingRemoteContextFailed, "DocumentLoader is null");
            }
            std::unique_ptr<RemoteDocument> rd;
            try {
                rd = options.getDocumentLoader()->loadDocument(importUri);
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
                result.erase(JsonLdConsts::BASE);
            } else if (value.is_string()) {
                // 5.7.3)
                // Otherwise, if value is an IRI, the base IRI of result is set to value.
                if (JsonLdUtils::isAbsoluteIri(value)) {
                    result.insert(std::make_pair(JsonLdConsts::BASE,
                                                 value.get<std::string>())); // todo: change code like this to use member vars in context?
                } else {
                    // 5.7.4)
                    // Otherwise, if value is a relative IRI reference and the base IRI of
                    // result is not null, set the base IRI of result to the result of resolving
                    // value against the current base IRI of result.
                    std::string baseUri = result.at(JsonLdConsts::BASE);
                    if (baseUri.empty())
                        throw JsonLdError(JsonLdError::InvalidBaseIri, "baseUri is empty");
                    std::string tmpIri = value.get<std::string>();
                    result.insert(std::make_pair(JsonLdConsts::BASE, JsonLdUrl::resolve(&baseUri, &tmpIri)));
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
                result.erase(JsonLdConsts::VOCAB);
            } else if (value.is_string()) {
                // 5.8.3)
                // Otherwise, if value is an IRI or blank node identifier, the vocabulary
                // mapping of result is set to the result of IRI expanding value using
                // true for document relative. If it is not an IRI, or a blank node
                // identifier, an invalid vocab mapping error has been detected and
                // processing is aborted.
                if (value.get<std::string>().empty() || BlankNode::isBlankNodeName(value) || JsonLdUtils::isIri(value)) {
                    std::string vocabMapping = result.expandIri(value.get<std::string>(), true, true);
                    result.insert(std::make_pair(JsonLdConsts::VOCAB, vocabMapping));
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
                result.erase(JsonLdConsts::LANGUAGE);
            } else if (value.is_string()) {
                // 5.9.3)
                // Otherwise, if value is a string, the default language of result is set
                // to value. If it is not a string, an invalid default language error has
                // been detected and processing is aborted.
                result.insert(
                        std::make_pair(JsonLdConsts::LANGUAGE, value.get<std::string>())); // todo: value to lowercase
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
            if (isProcessingMode(JsonLdOptions::JSON_LD_1_0))
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
                std::string directionStr = value;

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
            if (isProcessingMode(JsonLdOptions::JSON_LD_1_0))
                throw JsonLdError(JsonLdError::InvalidContextEntry);
            // 5.11.2)
            if (!context.at(JsonLdConsts::PROPAGATE).is_boolean()) {
                throw JsonLdError(JsonLdError::InvalidPropagateValue,
                                  R"(@propagate must be either true or false)");
            }
        }

        // 5.12)
        std::map<std::string, bool> defined;

        // 5.13)
        for (const auto &el : context.items()) {
            const auto &key = el.key();
            if (key == JsonLdConsts::BASE || key == JsonLdConsts::DIRECTION || key == JsonLdConsts::IMPORT ||
                key == JsonLdConsts::LANGUAGE || key == JsonLdConsts::PROPAGATE || key == JsonLdConsts::PROTECTED ||
                key == JsonLdConsts::VERSION || key == JsonLdConsts::VOCAB) {
                continue;
            }
            bool isProtected = false;
            if(context.contains(JsonLdConsts::PROTECTED))
                isProtected = context[JsonLdConsts::PROTECTED];
            result.createTermDefinition(context, key, defined, baseURL, isProtected, overrideProtected, remoteContexts);
        }

    }

    return result;
 }

std::string Context::getContainer(std::string property) {
    // todo: is this function valid anymore?
//        if (property == null) {
//            return null;
//        }
    if (property == JsonLdConsts::GRAPH) {
        return JsonLdConsts::SET;
    }
    if (property != JsonLdConsts::TYPE && JsonLdUtils::isKeyword(property)) {
        return property;
    }
    if(!termDefinitions.contains(property))
        return "";
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
  * @param value the Iri to expand
  * @param relative flag
  * @param vocab flag
  * @param context the context map
  * @param defined map of defined values
  * @return the expanded Iri
  * @throws JsonLdError
  */
std::string Context::expandIri(
        std::string value, bool relative, bool vocab,
        const json& context, std::map<std::string, bool> & defined) {
    // todo move to jsonldapi.cpp?

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the IRI expansion algorithm.
    // See: https://w3c.github.io/json-ld-api/#iri-expansion

    // todo: remove
    std::cout << "... 1.1 Context::expandIri()\n";

    // 1)
    // If value is a keyword or null, return value as is.
    if (JsonLdUtils::isKeyword(value)) { // todo: also checked for if value was null
        return value;
    }

    // 2)
    //  If value has the form of a keyword (i.e., it matches the ABNF rule "@"1*ALPHA
    //  from [RFC5234]), a processor SHOULD generate a warning and return null.
    if (JsonLdUtils::isKeywordForm(value)) { // todo: also checked for if value was null
        // todo: emit a warning
        return "";
    }

    // 3)
    // If local context is not null, it contains an entry with a key that equals value, and
    // the value of the entry for value in defined is not true, invoke the Create Term
    // Definition algorithm, passing active context, local context, value as term, and
    // defined. This will ensure that a term definition is created for value in active
    // context during Context Processing.
    if (!context.is_null() && context.contains(value)) {
        auto v = context.at(value);
        if(v.is_string() &&
           defined.find(v.get<std::string>()) != defined.end() &&
           !defined.at(v.get<std::string>())) {
            createTermDefinition(context, value, defined);
        }
    }

    // 4)
    // If active context has a term definition for value, and the associated IRI mapping
    // is a keyword, return that keyword.
    if (termDefinitions.find(value) != termDefinitions.end()) {
        auto td = termDefinitions.at(value);
        if (!td.is_null() &&
            td.contains(JsonLdConsts::ID) &&
            JsonLdUtils::isKeyword(td.at(JsonLdConsts::ID)))
            return td.at(JsonLdConsts::ID);
    }

    // 5)
    // If vocab is true and the active context has a term definition for value, return the
    // associated IRI mapping.
    if (vocab && termDefinitions.find(value) != termDefinitions.end()) {
        auto td = termDefinitions.at(value);
        if (!td.is_null() &&
            td.contains(JsonLdConsts::ID))
            return td.at(JsonLdConsts::ID);
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
        if (!context.is_null() && context.contains(prefix)
            && (defined.find(prefix) == defined.end() || !defined.at(prefix))) {
            createTermDefinition(context, prefix, defined);
        }

        // 6.4)
        // If active context contains a term definition for prefix having a non-null IRI mapping
        // and the prefix flag of the term definition is true, return the result of concatenating
        // the IRI mapping associated with prefix and suffix.
        if (termDefinitions.find(prefix) != termDefinitions.end()) {
            auto prefixDef = termDefinitions.at(prefix);
            if (prefixDef.find(JsonLdConsts::ID) != prefixDef.end()) {
                return prefixDef.at(JsonLdConsts::ID).get<std::string>() + suffix;
            }
        }

        // 6.5)
        // If value has the form of an IRI, return value.
        if(Uri::isAbsolute(value) || value.find_first_of("_:") == 0)
            return value;
    }

    // 7)
    // If vocab is true, and active context has a vocabulary mapping, return the result of
    // concatenating the vocabulary mapping with value.
    if (vocab && contextMap.find(JsonLdConsts::VOCAB) != contextMap.end()) {
        return contextMap.at(JsonLdConsts::VOCAB) + value;
    }

    // 8)
    // Otherwise, if document relative is true set value to the result of resolving value
    // against the base IRI from active context. Only the basic algorithm in section 5.2
    // of [RFC3986] is used; neither Syntax-Based Normalization nor Scheme-Based Normalization
    // are performed. Characters additionally allowed in IRI references are treated in the
    // same way that unreserved characters are treated in URI references, per section 6.5
    // of [RFC3987].
    else if (relative) {
        if(this->count(JsonLdConsts::BASE))
            return JsonLdUrl::resolve(&(this->at(JsonLdConsts::BASE)), &value);
        else
            return JsonLdUrl::resolve(nullptr, &value);
    } else if (!context.is_null() && JsonLdUtils::isRelativeIri(value)) {
        // todo: this isn't part of the spec--do we need it?
        throw JsonLdError(JsonLdError::InvalidIriMapping, "not an absolute IRI: " + value);
    }

    // 9)
    // Return value as is.
    return value;
}

std::string Context::expandIri(
        std::string value, bool relative, bool vocab) {
    // dummy objects
    json j;
    std::map<std::string, bool> m;
    return expandIri(std::move(value), relative, vocab, j, m);
}

/**
 * Create Term Definition Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#create-term-definition
 * https://w3c.github.io/json-ld-api/#create-term-definition
 *
 * @param context the context
 * @param term the term to define
 * @param defined map of defined values
 * @throws JsonLdError
 */
void Context::createTermDefinition(
        nlohmann::json context,
        const std::string& term,
        std::map<std::string, bool> & defined,
        std::string baseURL,
        bool isProtected,
        bool overrideProtected,
        std::vector<std::string> remoteContexts,
        bool validateScopedContext) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the create term definition algorithm.
    // See: https://w3c.github.io/json-ld-api/#create-term-definition

    // todo: remove
    std::cout << "... 1.1 Context::createTermDefinition()\n";

    // 1)
    // If defined contains the entry term and the associated value is true (indicating
    // that the term definition has already been created), return. Otherwise, if the
    // value is false, a cyclic IRI mapping error has been detected and processing is aborted.
    if (defined.find(term) != defined.end()) {
        if (defined.at(term)) {
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
    auto value = context.at(term);

    // 4)
    // If term is @type, and processing mode is json-ld-1.0, a keyword redefinition error has
    // been detected and processing is aborted.
    if(term == JsonLdConsts::TYPE) {
        if(isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
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
    if (termDefinitions.count(term)) {
        previousDefinition = termDefinitions[term];
        termDefinitions.erase(term);
    }

    bool simpleTerm = false;

    // 7)
    // If value is null, convert it to a map consisting of a single entry whose key
    // is @id and whose value is null.
    if(value.is_null()) {
        value = ObjUtils::newMap(JsonLdConsts::ID, nullptr);
    }

    // 8)
    // Otherwise, if value is a string, convert it to a map consisting of a single entry whose
    // key is @id and whose value is value. Set simple term to true.
    else if (value.is_string()) {
        value = ObjUtils::newMap(JsonLdConsts::ID, value);
        simpleTerm = true;
    }

    // 9)
    // Otherwise, value MUST be a map, if not, an invalid term definition error has been
    // detected and processing is aborted. Set simple term to false.
    else if (!(value.is_object())) {
        throw JsonLdError(JsonLdError::InvalidTermDefinition);
    }

    // 10)
    auto definition = ObjUtils::newMap();
    definition[JsonLdConsts::IS_PREFIX_FLAG] = false;
    definition[JsonLdConsts::IS_REVERSE_PROPERTY_FLAG] = false;
    definition[JsonLdConsts::IS_PROTECTED_FLAG] = isProtected;

    // 11)
    if (value.contains(JsonLdConsts::PROTECTED)) {
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
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
        auto type = value.at(JsonLdConsts::TYPE);

        // 12.1)
        // Initialize type to the value associated with the @type entry, which MUST be a
        // string. Otherwise, an invalid type mapping error has been detected and processing
        // is aborted.
        if (!(type.is_string())) {
            throw JsonLdError(JsonLdError::InvalidTypeMapping);
        }
        std::string typeStr = type.get<std::string>();

        // 12.2)
        // Set type to the result of IRI expanding type, using local context, and defined.
        try {
            typeStr = expandIri(typeStr, false, true, context, defined);
        } catch (JsonLdError &error) {
            std::string msg = error.what();
            if (msg.find(JsonLdError::InvalidIriMapping) == std::string::npos) {
                throw error;
            }
            throw JsonLdError(JsonLdError::InvalidTypeMapping, type);
        }

        // 12.3)
        // If the expanded type is @json or @none, and processing mode is json-ld-1.0, an invalid
        // type mapping error has been detected and processing is aborted.
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
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
        reverseStr = expandIri(reverseStr, false, true, context, defined);
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
        termDefinitions[term] = definition;
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
            idStr = expandIri(idStr, false, true, context, defined);
            // If the resulting IRI mapping is neither a keyword, nor an IRI, nor a blank node
            // identifier, an invalid IRI mapping error has been detected and processing is
            // aborted; if it equals @context, an invalid keyword alias error has been detected
            // and processing is aborted.
            if (JsonLdUtils::isKeyword(idStr) || JsonLdUtils::isAbsoluteIri(idStr) ||
                BlankNode::isBlankNodeName(idStr)) {
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
                std::string expandedTerm = expandIri(term, false, true, context, defined);
                if(expandedTerm.empty()) {
                    std::cout << "warning: expandedTerm.empty()\n"; // todo: remove if this doesn't happen
                }
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
                ((JsonLdUtils::isAbsoluteIri(definition[JsonLdConsts::ID]) &&
                  JsonLdUtils::iriEndsWithGeneralDelimiterCharacter(definition[JsonLdConsts::ID])) ||
                 BlankNode::isBlankNodeName(definition[JsonLdConsts::ID])))
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
        if (context.contains(prefix)) {
            createTermDefinition(context, prefix, defined);
        }
        // 15.2)
        // If term's prefix has a term definition in active context, set the IRI mapping of
        // definition to the result of concatenating the value associated with the prefix's
        // IRI mapping and the term's suffix.
        if (termDefinitions.find(prefix) != termDefinitions.end()) {
            auto id = termDefinitions.at(prefix).at(JsonLdConsts::ID);
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

        // 16.2)
        // Set the IRI mapping of definition to the result of IRI expanding term. If the
        // resulting IRI mapping is not an IRI, an invalid IRI mapping error has been detected
        // and processing is aborted.
        std::string expandedTerm = expandIri(term, false, true, context, defined);
        if(expandedTerm.empty()) {
            std::cout << "warning: expandedTerm.empty()\n"; // todo: remove if this doesn't happen
        }
        definition[JsonLdConsts::ID] = expandedTerm;
        if(!JsonLdUtils::isAbsoluteIri(definition[JsonLdConsts::ID])) {
            throw JsonLdError(JsonLdError::InvalidIriMapping,
                              "expanded term is not an IRI");
        }
    }
    // 17)
    else if (term == JsonLdConsts::TYPE) {
        definition[JsonLdConsts::ID] = JsonLdConsts::TYPE;
    }
    // 18)
    else if (contextMap.find(JsonLdConsts::VOCAB) != contextMap.end()) {
        definition[JsonLdConsts::ID] = contextMap.at(JsonLdConsts::VOCAB) + term;
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
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
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
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0) ||
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

        std::string indexStr = index.get<std::string>(); // todo will this just autoconvert to string?

        std::string expandedIndex = expandIri(indexStr, false, true, context, defined);

        if (expandedIndex.empty() || !JsonLdUtils::isAbsoluteIri(expandedIndex)) {
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
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
            throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
        }

        // 21.2)
        // Initialize context to the value associated with the @context entry, which is treated
        // as a local context.
        auto localContext = value.at(JsonLdConsts::CONTEXT);

        // 21.3)
        // Invoke the Context Processing algorithm using the active context, context as local
        // context, base URL, true for override protected, a copy of remote contexts, and false
        // for validate scoped context. If any error is detected, an invalid scoped context
        // error has been detected and processing is aborted.
        try {
            Context activeContext = *this;
            activeContext = activeContext.parse(localContext, baseURL, remoteContexts, true, propagate, false);

            // todo: need to wait on implementing this until after you update the main Context::parse() to store the
            // remotecontexts collection within Context. Then you can use it to make a local copy here for parsing this
            // JsonLdConsts::CONTEXT value.
            //throw JsonLdError(JsonLdError::NotImplemented);
        } catch (JsonLdError &error) {
            std::string msg = error.what();
            throw JsonLdError(JsonLdError::InvalidScopedContext,msg);
        }

        // 21.4)
        // Set the local context of definition to context, and base URL to base URL.
        definition[JsonLdConsts::LOCALCONTEXT] = localContext;
        definition[JsonLdConsts::BASEURL] = baseIRI;
    }

    // 22)
    if (value.contains(JsonLdConsts::LANGUAGE) && !value.contains(JsonLdConsts::TYPE)) {
        // 22.1)
        auto language = value.at(JsonLdConsts::LANGUAGE);

        if (language.is_null() || language.is_string()) {
            // todo: need to check if language is not well-formed, and if so, emit a warning
            // 22.2)
            definition[JsonLdConsts::LANGUAGE] = language; // todo: tolowercase or null?
        } else {
            throw JsonLdError(JsonLdError::InvalidLanguageMapping,
                              "@language must be a string or null");
        }
    }

    // 23.
    if (value.contains(JsonLdConsts::DIRECTION) && !value.contains(JsonLdConsts::TYPE)) {

        // 23.1, 23.2)
        auto direction = value.at(JsonLdConsts::DIRECTION);

        if (direction.is_null()) {
            definition[JsonLdConsts::DIRECTION] = direction;
        } else if (direction.is_string()) {

            std::string directionStr = direction;

            if (directionStr == "ltr" || directionStr == "rtl") {
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
    if (value.contains(JsonLdConsts::NEST)) {

        // 24.1)
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0)) {
            throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
        }

        // 24.2)
        auto nest = value.at(JsonLdConsts::NEST);

        if (!nest.is_string()) {
            throw JsonLdError(JsonLdError::InvalidNestValue,"");
        }

        std::string nestStr = nest;

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
        if (isProcessingMode(JsonLdOptions::JSON_LD_1_0) ||
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

        definition[JsonLdConsts::PREFIX] = prefix;

        // 25.3)
        // If the prefix flag of definition is set to true, and its IRI mapping is a
        // keyword, an invalid term definition has been detected and processing is aborted.
        if (definition[JsonLdConsts::PREFIX] && JsonLdUtils::isKeyword(definition[JsonLdConsts::ID])) {
            throw JsonLdError(JsonLdError::InvalidTermDefinition,"");
        }
    }

    // 26)
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
    if(!overrideProtected &&
       !previousDefinition.is_null() &&
       previousDefinition.contains(JsonLdConsts::IS_PROTECTED_FLAG) &&
       previousDefinition[JsonLdConsts::IS_PROTECTED_FLAG]) {
        // 27.1)
        // If definition is not the same as previous definition (other than the value
        // of protected), a protected term redefinition error has been detected, and
        // processing is aborted.
        json copyOfDefinition = definition;
        json copyOfPrevious = previousDefinition;
        std::cout << copyOfDefinition.dump() << std::endl;
        std::cout << copyOfPrevious.dump() << std::endl;
        copyOfDefinition.erase(JsonLdConsts::IS_PROTECTED_FLAG);
        copyOfPrevious.erase(JsonLdConsts::IS_PROTECTED_FLAG);
        if(copyOfDefinition != copyOfPrevious)
            throw JsonLdError(JsonLdError::ProtectedTermRedefinition);

        // 27.2)
        // Set definition to previous definition to retain the value of protected.
        definition = previousDefinition;
    }

    // 28)
    termDefinitions[term] = definition;
    defined[term] = true;

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
    // todo: shoul dmove this function and other slike it to a new TermDefinition class?
    if(!termDefinitions.count(property)) {
        return false;
    }
    auto td = termDefinitions.at(property);
    if (td.is_null()) {
        return false;
    }
    return td.contains(JsonLdConsts::REVERSE) && td.at(JsonLdConsts::REVERSE);
}

nlohmann::json Context::getTermDefinition(const std::string & key) {
    if(termDefinitions.count(key)) {
        return termDefinitions.at(key);
    }
    else
        return json::object();
}


json Context::expandValue(const std::string & activeProperty, const json& value)  {
    // todo move to jsonldapi.cpp?

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the Value Expansion algorithm.
    // See: https://w3c.github.io/json-ld-api/#value-expansion

    json result;
    json termDefinition = getTermDefinition(activeProperty);

    if(termDefinition.contains(JsonLdConsts::TYPE)) {

        std::string typeMapping = termDefinition.at(JsonLdConsts::TYPE);

        // 1)
        // If the active property has a type mapping in active context that is @id, and
        // the value is a string, return a new map containing a single entry where the key
        // is @id and the value is the result IRI expanding value using true for document
        // relative and false for vocab.
        if (typeMapping == JsonLdConsts::ID && value.is_string()) {
            result[JsonLdConsts::ID] = expandIri(value.get<std::string>(), true, false);
            return result;
        }

        // 2)
        // If active property has a type mapping in active context that is @vocab, and the
        // value is a string, return a new map containing a single entry where the key is @id
        // and the value is the result of IRI expanding value using true for document relative.
        if (typeMapping == JsonLdConsts::VOCAB && value.is_string()) {
            result[JsonLdConsts::ID] = expandIri(value.get<std::string>(), true, true);
            return result;
        }
    }

    // 3)
    // Otherwise, initialize result to a map with an @value entry whose value is set to value.
    result[JsonLdConsts::VALUE] = value;

    if(termDefinition.contains(JsonLdConsts::TYPE)) {

        std::string typeMapping = termDefinition.at(JsonLdConsts::TYPE);

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
                language = termDefinition.at(JsonLdConsts::LANGUAGE);
        }
        else if (contextMap.count(JsonLdConsts::LANGUAGE)) {
            language = contextMap.at(JsonLdConsts::LANGUAGE);
        }
        // 5.2)
        // Initialize direction to the direction mapping for active property in active
        // context, if any, otherwise to the default base direction of active context.
        std::string direction;
        if (termDefinition.contains(JsonLdConsts::DIRECTION)) {
            if(!termDefinition.at(JsonLdConsts::DIRECTION).is_null())
                direction = termDefinition.at(JsonLdConsts::DIRECTION);
        }
        else  {
            direction = getDefaultBaseDirection();
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

Context::Context(JsonLdOptions ioptions)
        : options(std::move(ioptions))
{
    init();
}

//Context::Context(std::map<std::string, std::string> map, JsonLdOptions ioptions)
//        : options(std::move(ioptions)), contextMap(std::move(map)) {
//    checkEmptyKey(contextMap);
//    init();
//}

//Context::Context(std::map<std::string, std::string> map)
//        : contextMap(std::move(map)) {
//    checkEmptyKey(contextMap);
//    init();
//}

bool Context::isProcessingMode(const std::string &mode) {
    return options.getProcessingMode() == mode;
}

const std::string &Context::getBaseIri() const {
    return baseIRI;
}

void Context::setBaseIri(const std::string &baseIri) {
    baseIRI = baseIri;
}

const std::string &Context::getOriginalBaseUrl() const {
    return originalBaseURL;
}

void Context::setOriginalBaseUrl(const std::string &originalBaseUrl) {
    originalBaseURL = originalBaseUrl;
}

Context *Context::getPreviousContext() const {
    return previousContext;
}

std::string Context::getDefaultBaseDirection() const {
    return defaultBaseDirection;
}

void Context::setDefaultBaseDirection(const std::string & direction) {
    defaultBaseDirection = direction;
}
