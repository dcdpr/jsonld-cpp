#ifndef LIBJSONLD_CPP_JSONLDUTILS_H
#define LIBJSONLD_CPP_JSONLDUTILS_H

#include "jsonld-cpp/jsoninc.h"

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

    bool isKeywordForm(const std::string& property);

    bool isAbsoluteIri(const std::string& iri);

    bool isRelativeIri(const std::string& iri);
    bool isIri(const std::string& iri);

    bool iriEndsWithGeneralDelimiterCharacter(const std::string& iri);

    bool isListObject(const json& j);
    bool isGraphObject(const json& j);

    bool isValueObject(const json& j);

    bool isObject(const json& j);
    bool isEmptyObject(const json& j);

    bool isArray(const json& j);
    bool isEmptyArray(const json& j);
    bool isArrayOfStrings(const json& j);

    bool isScalar(const json& j);
    bool isArrayOfScalars(const json& j);

    bool deepContains(const json& values, const json& value);

    void mergeValue(json & obj, const std::string& key, const json& value);

    void addValue(json & object, const std::string& key, const json& value, bool asArray);

    bool containsOrEquals(json & j, const std::string& value);
}

#endif //LIBJSONLD_CPP_JSONLDUTILS_H
