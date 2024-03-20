#ifndef LIBJSONLD_CPP_CONTEXTPROCESSOR_H
#define LIBJSONLD_CPP_CONTEXTPROCESSOR_H

#include <string>

#include "jsonld-cpp/detail/Context.h"
#include "jsonld-cpp/jsoninc.h"

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
            const nlohmann::ordered_json & localContext,
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


#endif //LIBJSONLD_CPP_CONTEXTPROCESSOR_H
