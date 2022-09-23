#ifndef LIBJSONLD_CPP_JSONLDAPI_H
#define LIBJSONLD_CPP_JSONLDAPI_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/Context.h"
#include "jsonld-cpp/RDFDataset.h"

class JsonLdApi {
private:
    JsonLdOptions options;
    BlankNodeNames blankNodeNames;

public:

    JsonLdApi() = default;
    explicit JsonLdApi(JsonLdOptions options);

    JsonLdOptions getOptions() const;

    /**
     * Expansion Algorithm
     *
     * This algorithm expands a JSON-LD document, such that all context definitions are
     * removed, all terms and compact IRIs are expanded to IRIs, blank node identifiers, or
     * keywords and all JSON-LD values are expressed in arrays in expanded form.
     *
     * https://www.w3.org/TR/json-ld11-api/#expansion-algorithm
     */
    nlohmann::json expand(
            Context activeContext,
            std::string *activeProperty,
            nlohmann::json element,
            const std::string & baseUrl,
            bool frameExpansion=false,
            bool ordered=false,
            bool fromMap=false);

    /**
     * Adds RDF triples for each graph in the current node map to an RDF
     * dataset.
     *
     * @return the RDF dataset.
     */
    RDF::RDFDataset toRDF(nlohmann::json element);

    /**
     * Performs RDF normalization on the given JSON-LD input.
     *
     * @param dataset
     *            the expanded JSON-LD object to normalize.
     * @return The normalized JSON-LD object
     * @throws JsonLdError
     *             If there was an error while normalizing.
     */
    std::string normalize(const RDF::RDFDataset& dataset);

private:

    nlohmann::json expandArrayElement(
            Context activeContext,
            std::string *activeProperty,
            const nlohmann::json& element,
            const std::string & baseUrl,
            bool frameExpansion,
            bool ordered,
            bool fromMap);

    nlohmann::json expandObjectElement(
            Context activeContext,
            std::string *activeProperty,
            nlohmann::json element,
            const std::string & baseUrl,
            nlohmann::json * propertyScopedContext,
            bool frameExpansion,
            bool ordered,
            bool fromMap);

    void generateNodeMap(nlohmann::json &element, nlohmann::json &nodeMap);

    void generateNodeMap(nlohmann::json &element, nlohmann::json &nodeMap, std::string *activeGraph,
                         nlohmann::json *activeSubject, std::string *activeProperty, nlohmann::json *list);

    std::string findInputType(Context &activeContext, Context &typeScopedContext, nlohmann::json &element);

    bool arrayContains(const nlohmann::json &containerMapping, const std::string &value) const;
};

#endif //LIBJSONLD_CPP_JSONLDAPI_H
