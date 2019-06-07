#include <utility>

#ifndef LIBJSONLD_CPP_JSONLDPROCESSOR_H
#define LIBJSONLD_CPP_JSONLDPROCESSOR_H

#include "json.hpp"
#include "JsonLdOptions.h"
#include "JsonLdError.h"
#include "Context.h"
#include "JsonLdApi.h"

/**
 * This class implements the <a href=
 * "http://json-ld.org/spec/latest/json-ld-api/#the-jsonldprocessor-interface" >
 * JsonLdProcessor interface</a>, except that it does not currently support
 * asynchronous processing, and hence does not return Promises, instead directly
 * returning the results.
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
    static nlohmann::json expand(std::string input, JsonLdOptions opts);

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



}

#endif //LIBJSONLD_CPP_JSONLDPROCESSOR_H
