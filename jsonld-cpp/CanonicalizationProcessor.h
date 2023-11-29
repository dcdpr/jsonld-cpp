#ifndef LIBJSONLD_CPP_CANONICALIZATIONPROCESSOR_H
#define LIBJSONLD_CPP_CANONICALIZATIONPROCESSOR_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdOptions.h"

namespace CanonicalizationProcessor {

    // Canonicalize JSON-LD or RDF document located at filename or IRI into
    // an RdfDataset according to the steps in the RDF Canonicalization Algorithm
    // See: https://w3c-ccg.github.io/rdf-dataset-canonicalization/spec/#canonicalization-algorithm
    std::string canonicalize(const std::string& documentLocation, JsonLdOptions& options);
}

#endif //LIBJSONLD_CPP_CANONICALIZATIONPROCESSOR_H
