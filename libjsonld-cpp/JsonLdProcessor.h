#ifndef LIBJSONLD_CPP_JSONLDPROCESSOR_H
#define LIBJSONLD_CPP_JSONLDPROCESSOR_H

#include "jsoninc.h"
#include "JsonLdOptions.h"
#include "JsonLdError.h"
#include "Context.h"
#include "RDFDataset.h"
#include "JsonLdApi.h"

/**
 * This class implements the
 * <a href="http://json-ld.org/spec/latest/json-ld-api/#the-jsonldprocessor-interface" >
 * JsonLdProcessor interface</a>.
 */

namespace JsonLdProcessor {

    /**
     * Expands the given input according to the steps in the
     * <a href="http://www.w3.org/TR/json-ld-api/#expansion-algorithm">Expansion
     * algorithm</a>.
     *
     * @param input
     *            The input JSON-LD object.
     * @param opts
     *            The {@link JsonLdOptions} that are to be sent to the expansion
     *            algorithm.
     * @return The expanded JSON-LD document
     * @throws JsonLdError
     *             If there is an error while expanding.
     */
    static nlohmann::json expand(nlohmann::json input, JsonLdOptions opts);
    static nlohmann::json expand(const std::string& input, JsonLdOptions opts);

    /**
     * Expands the given input according to the steps in the
     * <a href="http://www.w3.org/TR/json-ld-api/#expansion-algorithm">Expansion
     * algorithm</a>, using the default {@link JsonLdOptions}.
     *
     * @param input
     *            The input JSON-LD object.
     * @return The expanded JSON-LD document
     * @throws JsonLdError
     *             If there is an error while expanding.
     */
    static nlohmann::json expand(nlohmann::json input)  {
        JsonLdOptions opts;
        return expand(std::move(input), opts);
    }
    static nlohmann::json expand(std::string input)  {
        JsonLdOptions opts;
        return expand(std::move(input), opts);
    }

    static RDF::RDFDataset toRDF(const std::string& input, const JsonLdOptions& options);
    static std::string toRDFString(const std::string& input, const JsonLdOptions& options);

    static std::string normalize(const std::string& input, const JsonLdOptions& options);
}

#endif //LIBJSONLD_CPP_JSONLDPROCESSOR_H
