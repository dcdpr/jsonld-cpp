#ifndef LIBJSONLD_CPP_JSONLDUTILS_H
#define LIBJSONLD_CPP_JSONLDUTILS_H

#include "json.hpp"


namespace JsonLdUtils {

    using json = nlohmann::json;

    bool deepCompare(json v1, json v2);

    /**
     * Returns whether or not the given value is a keyword (or a keyword alias).
     *
     * @param property
     *            the value to check.
     * @return true if the value is a keyword, false if not.
     */
    bool isKeyword(std::string property);

    bool isAbsoluteIri(const std::string& value);

    bool isRelativeIri(const std::string& value);

    bool isList(json j);

    bool isValue(json j);

    bool isObject(json j);

    bool deepContains(json values, json value);

    void mergeValue(json & obj, std::string key, json value);


}

#endif //LIBJSONLD_CPP_JSONLDUTILS_H
