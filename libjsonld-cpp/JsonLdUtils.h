#ifndef LIBJSONLD_CPP_JSONLDUTILS_H
#define LIBJSONLD_CPP_JSONLDUTILS_H

#include "jsoninc.h"

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
    bool isKeyword(const std::string& property);

    bool isAbsoluteIri(const std::string& value);

    bool isRelativeIri(const std::string& value);

    bool isList(const json& j);

    bool isValue(const json& j);

    bool isObject(const json& j);

    bool deepContains(const json& values, const json& value);

    void mergeValue(json & obj, const std::string& key, const json& value);
}

#endif //LIBJSONLD_CPP_JSONLDUTILS_H
