#ifndef LIBJSONLD_CPP_RDFSERIALIZATIONPROCESSOR_H
#define LIBJSONLD_CPP_RDFSERIALIZATIONPROCESSOR_H

#include "jsonld-cpp/jsoninc.h"


class JsonLdOptions;

namespace RDF {
    class RDFDataset;
}

struct RDFSerializationProcessor {

    /**
     * Deserialize JSON-LD to RDF Algorithm
     *
     * This algorithm deserializes a JSON-LD document to an RDF dataset. Please note that RDF
     * does not allow a blank node to be used as a property, while JSON-LD does. Therefore, by default
     * triples that would have contained blank nodes as properties are discarded when interpreting
     * JSON-LD as RDF.
     *
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldprocessor-tordf
     */
    static RDF::RDFDataset toRDF(nlohmann::ordered_json expandedInput, const JsonLdOptions& options);


};


#endif //LIBJSONLD_CPP_RDFSERIALIZATIONPROCESSOR_H
