#ifndef JSONLD_CPP_LIBRARY_EXPANSIONPROCESSOR_H
#define JSONLD_CPP_LIBRARY_EXPANSIONPROCESSOR_H

#include "jsonld-cpp/jsoninc.h"

class Context;

struct ExpansionProcessor {

    /**
     * Expansion Algorithm
     *
     * This algorithm expands a JSON-LD document, such that all context definitions are
     * removed, all terms and compact IRIs are expanded to IRIs, blank node identifiers, or
     * keywords and all JSON-LD values are expressed in arrays in expanded form.
     *
     * https://www.w3.org/TR/json-ld11-api/#expansion-algorithm
     *
     * @param activeContext Currently active Context used to resolve terms.
     * @param activeProperty Currently active property or keyword. Can be null.
     * @param element The element to be expanded.
     * @param baseUrl URL associated with the documentUrl of the original document to expand.
     * @param fromMap Flag to control reverting previous term definitions in the active context
     *                associated with non-propagated contexts. Defaults to false.
     * @return Fully expanded JSON-LD document.
     */
    static nlohmann::ordered_json expand(
            Context activeContext,
            const std::string *activeProperty,
            nlohmann::ordered_json element,
            const std::string & baseUrl,
            bool fromMap=false);

};


#endif //JSONLD_CPP_LIBRARY_EXPANSIONPROCESSOR_H
