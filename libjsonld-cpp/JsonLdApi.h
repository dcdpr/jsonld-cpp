#include <utility>

#ifndef LIBJSONLD_CPP_JSONLDAPI_H
#define LIBJSONLD_CPP_JSONLDAPI_H

#include "JsonLdOptions.h"
#include "Context.h"
#include "RDFDataset.h"
#include "json.hpp"

class JsonLdApi {
private:
    nlohmann::json value;
    JsonLdOptions options;
    BlankNodeIdentifierGenerator blankNodeIdGenerator;

public:

    JsonLdApi() = default;

    explicit JsonLdApi(JsonLdOptions options)
            : options(std::move(options)) {
    }

    JsonLdApi(nlohmann::json input, JsonLdOptions options)
            : value(std::move(input)), options(std::move(options)) {
    }

    JsonLdOptions getOptions() const {
        return options;
    }

    /**
 * Expansion Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#expansion-algorithm
 *
 * @param activeCtx
 *            The Active Context
 * @param activeProperty
 *            The Active Property
 * @param element
 *            The current element
 * @return The expanded JSON-LD object.
 */
    nlohmann::json expand(Context activeCtx, std::string *activeProperty, nlohmann::json element);

    /**
 * Expansion Algorithm
 *
 * http://json-ld.org/spec/latest/json-ld-api/#expansion-algorithm
 *
 * @param activeCtx
 *            The Active Context
 * @param element
 *            The current element
 * @return The expanded JSON-LD object.
 */
    nlohmann::json expand(Context activeCtx, nlohmann::json element);

    /**
 * Adds RDF triples for each graph in the current node map to an RDF
 * dataset.
 *
 * @return the RDF dataset.
 */
    RDF::RDFDataset toRDF();


private:

    nlohmann::json expandArrayElement(Context activeCtx, std::string *activeProperty, nlohmann::json element);

    nlohmann::json expandObjectElement(Context activeCtx, std::string *activeProperty, nlohmann::json element);

    void generateNodeMap(nlohmann::json &element, nlohmann::json &nodeMap);

    void generateNodeMap(nlohmann::json &element, nlohmann::json &nodeMap, std::string *activeGraph,
                         nlohmann::json *activeSubject, std::string *activeProperty, nlohmann::json *list);
};


#endif //LIBJSONLD_CPP_JSONLDAPI_H
