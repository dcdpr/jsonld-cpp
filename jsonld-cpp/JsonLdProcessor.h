#ifndef LIBJSONLD_CPP_JSONLDPROCESSOR_H
#define LIBJSONLD_CPP_JSONLDPROCESSOR_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdOptions.h"

namespace RDF {
    class RDFDataset;
}

/**
 * Functions in this namespace implement the JsonLdProcessor interface.
 * See: https://www.w3.org/TR/json-ld11-api/#the-jsonldprocessor-interface
 */

namespace JsonLdProcessor {

    // Expands JSON-LD document located at filename or IRI according to the steps in the
    // Expansion algorithm
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-expand
    // See: https://www.w3.org/TR/json-ld11-api/#expansion-algorithm
    nlohmann::json expand(const std::string& documentLocation, JsonLdOptions& options);

    // Transforms JSON-LD document located at filename or IRI into an RdfDataset
    // according to the steps in the Deserialize JSON-LD to RDF Algorithm
    // See: https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-tordf
    // See: https://www.w3.org/TR/json-ld11-api/#deserialize-json-ld-to-rdf-algorithm
    RDF::RDFDataset toRDF(const std::string& documentLocation, JsonLdOptions& options);

    // Normalizes (canonicalizes) JSON-LD or RDF document located at filename or IRI into
    // an RdfDataset according to the steps in the RDF Canonicalization Algorithm
    // See: https://w3c-ccg.github.io/rdf-dataset-canonicalization/spec/#canonicalization-algorithm
    std::string normalize(const std::string& documentLocation, JsonLdOptions& options);
}

#endif //LIBJSONLD_CPP_JSONLDPROCESSOR_H
