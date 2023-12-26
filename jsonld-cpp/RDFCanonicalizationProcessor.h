#ifndef JSONLD_CPP_LIBRARY_RDFCANONICALIZATIONPROCESSOR_H
#define JSONLD_CPP_LIBRARY_RDFCANONICALIZATIONPROCESSOR_H

#include <string>

class JsonLdOptions;
namespace RDF {
    class RDFDataset;
}

struct RDFCanonicalizationProcessor {

    /**
     * Canonicalization Algorithm
     *
     * This algorithm converts an input RDFDataset into a canonicalized RDFDataset. This algorithm
     * will assign deterministic identifiers to any blank nodes in the input dataset.
     *
     * https://w3c-ccg.github.io/rdf-dataset-canonicalization/spec/#canonicalization-algorithm
     */
    static std::string canonicalize(const RDF::RDFDataset& dataset, const JsonLdOptions& options);

    /**
     * Canonicalization Algorithm
     *
     * This algorithm converts a JSON-LD or RDF document located at filename or IRI into a
     * canonicalized RDFDataset. This algorithm will assign deterministic identifiers to
     * any blank nodes in the input dataset.
     *
     * https://w3c-ccg.github.io/rdf-dataset-canonicalization/spec/#canonicalization-algorithm
     */
    static std::string canonicalize(const std::string& documentLocation, JsonLdOptions& options);

};


#endif //JSONLD_CPP_LIBRARY_RDFCANONICALIZATIONPROCESSOR_H
