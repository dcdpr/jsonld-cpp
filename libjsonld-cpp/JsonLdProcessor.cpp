#include "JsonLdProcessor.h"
#include "RDFDataset.h"
#include "RDFDatasetUtils.h"

using RDF::RDFDataset;
using nlohmann::json;

nlohmann::json JsonLdProcessor::expand(nlohmann::json input, JsonLdOptions opts) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the JsonLdProcessor interface.
    // See: https://w3c.github.io/json-ld-api/#the-jsonldprocessor-interface

    // todo: remove
    std::cout << "... 1.0 JsonLdProcessor::expand2()\n";

    // 1)
    // todo: set promise?

    // 2)
    // todo: remoteDocument?

    // 3)
    // todo: input is string? already handled in another version of expand

    // 4)
    // todo: document from remote is a string ... that's kind of what this versino already is

    // 5)
    Context activeCtx(opts);
    activeCtx.setBaseIri(opts.getBase()); // todo: use documentUrl from remoteDocument if available
    activeCtx.setOriginalBaseUrl(opts.getBase()); // todo: use documentUrl from remoteDocument if available

    // 6)
    if (!opts.getExpandContext().empty()) {
        json expandContext = opts.getExpandContext();
        if (expandContext.contains(JsonLdConsts::CONTEXT)) {
            expandContext = expandContext[JsonLdConsts::CONTEXT];
        }
        activeCtx = activeCtx.parse(expandContext, activeCtx.getOriginalBaseUrl());
    }

    // 7)
    // todo: if remotedocument has a contextUrl ...


    // 8)
    JsonLdApi api(opts);
    json expandedOutput = api.expand(
            activeCtx,
            nullptr,
            input,
            activeCtx.getOriginalBaseUrl()); // todo: use documentUrl from remoteDocument if available

    // 8.1)
    if (expandedOutput.is_object() && expandedOutput.contains(JsonLdConsts::GRAPH)
        && expandedOutput.size() == 1) {
        expandedOutput = expandedOutput.at(JsonLdConsts::GRAPH);
    }
    // 8.2)
    else if (expandedOutput.is_null()) {
        expandedOutput = json::array();
    }

    // 8.3)
    if (!expandedOutput.is_array()) {
        json tmp = json::array();
        tmp.push_back(expandedOutput);
        expandedOutput = tmp;
    }

    return expandedOutput;
}

nlohmann::json JsonLdProcessor::expand(const std::string& input, JsonLdOptions opts) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the JsonLdProcessor interface.
    // See: https://www.w3.org/TR/2014/REC-json-ld-api-20140116/#the-jsonldprocessor-interface

    // todo: This needs to be upgraded to conform with
    // https://w3c.github.io/json-ld-api/#the-jsonldprocessor-interface

    // todo: remove
    std::cout << "... 1.0 JsonLdProcessor::expand1()\n";

    // 2) TODO: better verification of DOMString IRI
    if (input.find(':') != std::string::npos) {
        try {
            auto tmp = opts.getDocumentLoader()->loadDocument(input);
            const json& json_input = tmp->getJSONContent();
            // TODO: figure out how to deal with remote context

            // if set the base in options should override the base iri in the
            // active context
            // thus only set this as the base iri if it's not already set in
            // options
            if (opts.getBase().empty()) {
                opts.setBase(input);
            }

            return expand(json_input, opts);

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


RDFDataset JsonLdProcessor::toRDF(const std::string& input, const JsonLdOptions& options) {

    nlohmann::json expandedInput = expand(input, options);

    JsonLdApi api(options);
    RDFDataset dataset = api.toRDF(expandedInput);

//
//    // generate namespaces from context
//    if (options.useNamespaces) {
//        List<Map<String, Object>> _input;
//        if (input instanceof List) {
//            _input = (List<Map<String, Object>>) input;
//        } else {
//            _input = new ArrayList<Map<String, Object>>();
//            _input.add((Map<String, Object>) input);
//        }
//        for (final Map<String, Object> e : _input) {
//            if (e.containsKey(JsonLdConsts.CONTEXT)) {
//                dataset.parseContext(e.get(JsonLdConsts.CONTEXT));
//            }
//        }
//    }
//
//    if (callback != null) {
//        return callback.call(dataset);
//    }
//
    return dataset;
}

std::string JsonLdProcessor::toRDFString(const std::string& input, const JsonLdOptions& options) {

    nlohmann::json expandedInput = expand(input, options);

    JsonLdApi api(options);
    RDFDataset dataset = api.toRDF(expandedInput);
    // todo: while present in the java version, none of the toRdf() tests needed this namespace
    // stuff, so come back to it
//    // generate namespaces from context
//    if (options.useNamespaces) {
//        List<Map<String, Object>> _input;
//        if (input instanceof List) {
//            _input = (List<Map<String, Object>>) input;
//        } else {
//            _input = new ArrayList<Map<String, Object>>();
//            _input.add((Map<String, Object>) input);
//        }
//        for (final Map<String, Object> e : _input) {
//            if (e.containsKey(JsonLdConsts.CONTEXT)) {
//                dataset.parseContext(e.get(JsonLdConsts.CONTEXT));
//            }
//        }
//    }
//
//    if (callback != null) {
//        return callback.call(dataset);
//    }
//
    return RDFDatasetUtils::toNQuads(dataset);
}

std::string JsonLdProcessor::normalize(const std::string& input, const JsonLdOptions& options) {

    JsonLdApi api(options);
    auto tmp = options.getDocumentLoader()->loadDocument(input);

    if(tmp->getContentType() == MediaType::json_ld()) {
        RDFDataset dataset = toRDF(input, options);
        return api.normalize(dataset);
    }
    else if(tmp->getContentType() == MediaType::n_quads()) {
        RDFDataset dataset = tmp->getRDFContent();
        return api.normalize(dataset);
    }
    else {
        std::stringstream ss;
        ss << "Unsupported content type: '" << tmp->getContentType()
           << "'. Supported content types for normalization are: "
           << MediaType::json_ld() << " and " << MediaType::n_quads();
        throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
    }

}

