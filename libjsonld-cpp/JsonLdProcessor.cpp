#include "JsonLdProcessor.h"
#include "RDFDataset.h"
#include "RDFDatasetUtils.h"

using RDF::RDFDataset;
using nlohmann::json;

nlohmann::json JsonLdProcessor::expand(nlohmann::json input, JsonLdOptions opts) {

    // Comments in this function are labelled with numbers that correspond to sections
    // from the description of the JsonLdProcessor interface.
    // See: https://www.w3.org/TR/2014/REC-json-ld-api-20140116/#the-jsonldprocessor-interface

    // todo: This needs to be upgraded to conform with
    // https://w3c.github.io/json-ld-api/#the-jsonldprocessor-interface

    // todo: remove
    std::cout << "... 1.0 JsonLdProcessor::expand2()\n";

    // 3)
    Context activeCtx(opts);

    // 4)
    if (!opts.getExpandContext().empty()) {
        json exCtx = opts.getExpandContext();
        if (exCtx.contains(JsonLdConsts::CONTEXT)) {
            exCtx = exCtx[JsonLdConsts::CONTEXT];
        }
        activeCtx = activeCtx.parse(exCtx);
    }

    // 5)
    // TODO: add support for getting a context from HTTP when content-type
    // is set to a jsonld compatible format

    // 6)
    JsonLdApi api(opts);
    json expanded = api.expand(activeCtx, input);

    // final step of Expansion Algorithm
    if (expanded.is_object() && expanded.contains(JsonLdConsts::GRAPH)
        && expanded.size() == 1) {
        expanded = expanded.at(JsonLdConsts::GRAPH);
    } else if (expanded.is_null()) {
        expanded = json::array();
    }

    // normalize to an array
    if (!expanded.is_array()) {
        json tmp = json::array();
        tmp.push_back(expanded);
        expanded = tmp;
    }

    return expanded;
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

nlohmann::json JsonLdProcessor::expand(nlohmann::json input) {
    JsonLdOptions opts;
    return expand(std::move(input), opts);
}

nlohmann::json JsonLdProcessor::expand(std::string input) {
    JsonLdOptions opts;
    return expand(std::move(input), opts);
}
