#ifndef JSONLD_CPP_LIBRARY_CONTEXTPROCESSOR_H
#define JSONLD_CPP_LIBRARY_CONTEXTPROCESSOR_H

#include "jsonld-cpp/Context.h"
#include "jsonld-cpp/jsoninc.h"
#include <string>

struct ContextProcessor {

    /**
     * Context Processing Algorithm
     *
     * https://www.w3.org/TR/json-ld11-api/#context-processing-algorithm
     *
     * @return The parsed and merged Context.
     * @throws JsonLdError
     *             If there is an error parsing the contexts.
     */
    static Context process(
            const Context & activeContext,
            const nlohmann::json & localContext,
            const std::string & baseURL,
            bool overrideProtected = false,
            bool propagate = true,
            bool validateScopedContext = true);

    /**
      * IRI Expansion Algorithm
      *
      * https://www.w3.org/TR/json-ld11-api/#iri-expansion
      *
      * @return the expanded Iri
      * @throws JsonLdError
      *             If there is an error expanding the IRI.
      */
    static std::string expandIri(Context & activeContext, std::string value, bool relative, bool vocab);

};


#endif //JSONLD_CPP_LIBRARY_CONTEXTPROCESSOR_H
