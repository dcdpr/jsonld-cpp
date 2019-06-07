#include "JsonLdProcessor.h"

nlohmann::json JsonLdProcessor::expand(nlohmann::json input, JsonLdOptions opts) {

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

nlohmann::json JsonLdProcessor::expand(std::string input, JsonLdOptions opts) {

    // 1)
    // TODO: look into java futures/promises

    // 2) TODO: better verification of DOMString IRI
    auto p = input.find(':');
auto q = std::string::npos;
//    if (!(p == q)) {
        try {
            RemoteDocument tmp = opts.getDocumentLoader().loadDocument(input);
            json json_input = tmp.getDocument();
            // TODO: figure out how to deal with remote context

            // if set the base in options should override the base iri in the
            // active context
            // thus only set this as the base iri if it's not already set in
            // options
            if (opts.getBase() == "") {
                opts.setBase(input);
            }

            return expand(json_input, opts);

        }
        catch (const std::exception &e) {
            throw JsonLdError(JsonLdError::LoadingDocumentFailed, e.what());
        }

//    }
}

