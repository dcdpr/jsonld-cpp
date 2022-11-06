#include "jsonld-cpp/JsonLdProcessor.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/Context.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/ExpansionProcessor.h"
#include "jsonld-cpp/RDFSerializationProcessor.h"
#include "jsonld-cpp/RDFCanonicalizationProcessor.h"
#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/ContextProcessor.h"

using RDF::RDFDataset;
using nlohmann::json;

nlohmann::json JsonLdProcessor::expand(const nlohmann::json & input, JsonLdOptions& options) {

    // Comments in this function are labeled with numbers that correspond to sections
    // from the description of the expand() function in the JsonLdProcessor interface.
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-expand

    // 1)
    // Create a new Promise promise and return it. The following steps are then deferred.
    // todo: promise?

    // 2)
    // If the provided input is a RemoteDocument, initialize remote document to input.
    // todo: other function

    // 3)
    // Otherwise, if the provided input is a string representing the IRI of a remote
    // document, await and dereference it as remote document using LoadDocumentCallback, passing
    // input for url, the extractAllScripts option from options for extractAllScripts.
    // todo: other function

    // 4)
    // If document from remote document is a string, transform into the internal
    // representation. If document cannot be transformed to the internal representation, reject
    // promise passing a loading document failed error.
    // todo: ... clean this up. ... 'input' is already a json object

    // 5)
    // Initialize a new empty active context. The base IRI and original base URL of the active
    // context is set to the documentUrl from remote document, if available; otherwise to the
    // base option from options. If set, the base option from options overrides the base IRI.
    Context activeContext(options);
    activeContext.setBaseIri(options.getBase());
    // todo: documentUrl
    activeContext.setOriginalBaseUrl(options.getBase());

    // 6)
    // If the expandContext option in options is set, update the active context using the
    // Context Processing algorithm, passing the expandContext as local context and the original
    // base URL from active context as base URL. If expandContext is a map having an @context
    // entry, pass that entry's value instead for local context.
    if (!options.getExpandContext().empty()) {
        json expandContext = options.getExpandContext();
        if (expandContext.contains(JsonLdConsts::CONTEXT)) {
            expandContext = expandContext[JsonLdConsts::CONTEXT];
        }
        activeContext = ContextProcessor::process(activeContext, expandContext, activeContext.getOriginalBaseUrl());
    }

    // 7)
    // If remote document has a contextUrl, update the active context using the Context
    // Processing algorithm, passing the contextUrl as local context, and contextUrl as base URL.
    // todo: contextUrl ...

    // 8)
    // Set expanded output to the result of using the Expansion algorithm, passing the active
    // context, document from remote document or input if there is no remote document as
    // element, null as active property, documentUrl as base URL, if available, otherwise to
    // the base option from options, and the frameExpansion and ordered flags from options.
    json expandedOutput = ExpansionProcessor::expand(
            activeContext,
            nullptr,
            input,
            activeContext.getOriginalBaseUrl());

    // 8.1)
    // If expanded output is a map that contains only an @graph entry, set expanded output to
    // that value.
    if (expandedOutput.is_object() && expandedOutput.contains(JsonLdConsts::GRAPH)
        && expandedOutput.size() == 1) {
        expandedOutput = expandedOutput.at(JsonLdConsts::GRAPH);
    }

    // 8.2)
    // If expanded output is null, set expanded output to an empty array.
    if (expandedOutput.is_null()) {
        expandedOutput = json::array();
    }

    // 8.3)
    // If expanded output is not an array, set expanded output to an array containing only
    // expanded output.
    if (!expandedOutput.is_array()) {
        json tmp = json::array();
        tmp.push_back(expandedOutput);
        expandedOutput = tmp;
    }

    // 9)
    // Resolve the promise with expanded output transforming expanded output from the internal
    // representation to a JSON serialization.
    return expandedOutput;
}

nlohmann::json JsonLdProcessor::expand(const std::string& documentLocation, JsonLdOptions& options) {

    // Comments in this function are labeled with numbers that correspond to sections
    // from the description of the expand() function in the JsonLdProcessor interface.
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-expand

    // 1)
    // ...

    // 2)
    // ...

    // 3)
    // Otherwise, if the provided input is a string representing the IRI of a remote
    // document, await and dereference it as remote document using LoadDocumentCallback, passing
    // input for url, the extractAllScripts option from options for extractAllScripts.
    if (documentLocation.find(':') != std::string::npos) {
        try {
            auto tmp = options.getDocumentLoader()->loadDocument(documentLocation);
            const json& json_input = tmp->getJSONContent();

            if (options.getBase().empty()) {
                options.setBase(documentLocation);
            }

            // All other steps in JsonLdProcessor.expand interface are continued here
            return expand(json_input, options);

        }
        catch (const JsonLdError &e) {
            throw e;
        }
        catch (const std::exception &e) {
            throw JsonLdError(JsonLdError::LoadingDocumentFailed, e.what());
        }
    }
    else
        return json::array(); // todo: what else should happen?
}

RDFDataset JsonLdProcessor::toRDF(const std::string& documentLocation, JsonLdOptions& options) {

    // Comments in this function are labeled with numbers that correspond to sections
    // from the description of the toRDF() function in the JsonLdProcessor interface.
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-tordf

    // 1)
    // Create a new Promise promise and return it. The following steps are then deferred.
    // todo: promise?

    // 2)
    // Set expanded input to the result of using the expand() method using input and options
    // with ordered set to false.
    nlohmann::json expandedInput = expand(documentLocation, options);

    // 3-7)
    // Rest of the algorithm in toRDF().
    RDFDataset dataset = RDFSerializationProcessor::toRDF(expandedInput, options);

    return dataset;
}

std::string JsonLdProcessor::normalize(const std::string& documentLocation, JsonLdOptions& options) {

    auto document = options.getDocumentLoader()->loadDocument(documentLocation);

    if(document->getContentType() == MediaType::json_ld()) {
        RDFDataset dataset = toRDF(documentLocation, options);
        return RDFCanonicalizationProcessor::normalize(dataset, options);
    }
    else if(document->getContentType() == MediaType::n_quads()) {
        RDFDataset dataset = document->getRDFContent();
        return RDFCanonicalizationProcessor::normalize(dataset, options);
    }
    else {
        std::stringstream ss;
        ss << "Unsupported content type: '" << document->getContentType()
           << "'. Supported content types for normalization are: "
           << MediaType::json_ld() << " and " << MediaType::n_quads();
        throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
    }

}

