#include <utility>

#ifndef LIBJSONLD_CPP_JSONLDAPI_H
#define LIBJSONLD_CPP_JSONLDAPI_H

#include "JsonLdOptions.h"
#include "Context.h"

class JsonLdApi {
private:
    JsonLdOptions options;

public:

    JsonLdApi() {
//        options = std::shared_ptr<JsonLdOptions>(new JsonLdOptions());
    }

    explicit JsonLdApi(JsonLdOptions options)
            : options(std::move(options))
    {
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
 * @throws JsonLdError
 *             If there was an error during expansion.
 */
    json expand(Context activeCtx, std::string * activeProperty, json element);

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
 * @throws JsonLdError
 *             If there was an error during expansion.
 */
    json expand(Context activeCtx, json element);


    json expandArrayElement(Context activeCtx, std::string * activeProperty, json element);
    json expandObjectElement(Context activeCtx, std::string * activeProperty, json element);
};


#endif //LIBJSONLD_CPP_JSONLDAPI_H
