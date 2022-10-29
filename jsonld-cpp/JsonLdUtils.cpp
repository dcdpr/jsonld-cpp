#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/Uri.h"
#include <set>
#include <regex>

using namespace JsonLdConsts;

namespace {

    std::set<std::string> knownKeywords = {
            // See: https://www.w3.org/TR/json-ld11/#keywords for normative list.
      BASE,
      CONTAINER,
      CONTEXT,
      DIRECTION,
      GRAPH,
      ID,
      IMPORT,
      INCLUDED,
      INDEX,
      JSON,
      LANGUAGE,
      LIST,
      NEST,
      NONE,
      PREFIX,
      PROPAGATE,
      PROTECTED,
      REVERSE,
      SET,
      TYPE,
      VALUE,
      VERSION,
      VOCAB,
            // See: https://w3c.github.io/json-ld-api/#api-keywords
      PRESERVE,
            // See: https://www.w3.org/TR/json-ld11-framing/#framing-keywords
      DEFAULT,
      EMBED,
      "@always",
      "@once",
      "@never",
      EXPLICIT,
      ATNULL,
      OMIT_DEFAULT,
      REQUIRE_ALL,
            // these are other keywords found reading through the spec. Not sure why they aren't
            // included in the above.
      "@any"
    };
}

bool JsonLdUtils::isKeyword(const std::string& property) {
    return knownKeywords.count(property) > 0;
}

bool JsonLdUtils::isKeywordForm(const std::string& property) {
    std::regex keywordForm(R"(@[A-Za-z]+)");
    std::smatch match;
    return std::regex_match(property, match, keywordForm);
}

bool JsonLdUtils::isAbsoluteIri(const std::string &iri) {
    return Uri::isAbsolute(iri);
}

bool JsonLdUtils::isRelativeIri(const std::string &iri) {
    return !(isKeyword(iri) || isAbsoluteIri(iri)); // todo is this a good test?
}

bool JsonLdUtils::isIri(const std::string &iri) {
    return !isKeyword(iri) && Uri::isUri(iri); // todo is this a good test?
}

bool JsonLdUtils::deepCompare(json v1, json v2) {
    if (v1.is_null()) {
        return v2.is_null();
    } else if (v2.is_null()) {
        return v1.is_null();
    } else if (v1.is_object() && v2.is_object()) {
        if (v1.size() != v2.size()) {
            return false;
        }
        for (auto& el : v1.items()) {
            if(!v2.contains(el.key()) || !deepCompare(el.value(), v2.at(el.key())))
                return false;
        }
        return true;
    } else if (v1.is_array() && v2.is_array()) {
        if (v1.size() != v2.size()) {
            return false;
        }
        // used to mark members of v2 that we have already matched to avoid
        // matching the same item twice for lists that have duplicates
        std::vector<bool> alreadyMatched(v2.size());
        for (const auto& o1 : v1) {
            bool gotmatch = false;

            for (size_t j = 0; j < v2.size(); j++) {
                if (!alreadyMatched[j] && deepCompare(o1, v2.at(j))) {
                    alreadyMatched[j] = true;
                    gotmatch = true;
                    break;
                }
            }

            if (!gotmatch) {
                return false;
            }
        }
        return true;
    } else {
        return v1 == v2;
    }
}

bool JsonLdUtils::isListObject(const json& j) {
    return JsonLdUtils::isObject(j) &&
           j.contains(LIST) &&
           (j.size() == 1 || (j.size() == 2 && j.contains(INDEX)));
}

bool JsonLdUtils::isGraphObject(const json& j) {
    if(JsonLdUtils::isObject(j) && j.contains(GRAPH)) {
        std::set<std::string> validKeywords {GRAPH, ID, INDEX};
        for (auto& el : j.items()) {
            if(validKeywords.find(el.key()) == validKeywords.end())
                return false;
        }
        return true;
    }
    return false;
}

bool JsonLdUtils::isValueObject(const json& j) {
    return JsonLdUtils::isObject(j) && j.contains(VALUE);
}

bool JsonLdUtils::isObject(const json& j) {
    return j.is_object();
}

bool JsonLdUtils::isEmptyObject(const json& j) {
    return j.is_object() && j.empty();
}

/**
 * Check if a json object is a default object
 *
 * See: https://www.w3.org/TR/json-ld11/#dfn-default-j
 *
 * @param j the object to test
 * @return true if j is a default object
 */
bool JsonLdUtils::isDefaultObject(const json& j) {
    return j.is_object() && j.contains(DEFAULT);
}


/**
 * Check if a json object is a node object
 *
 * See: https://www.w3.org/TR/json-ld11/#dfn-node-j
 *
 * @param j the object to test
 * @return true if j is a node object
 */
bool JsonLdUtils::isNodeObject(const json& j) {
    return j.is_object() &&
           ((!j.contains(VALUE) &&
             !j.contains(LIST) &&
             !j.contains(SET)) ||
            (j.contains(CONTEXT) &&
             j.contains(GRAPH))
           );
}

bool JsonLdUtils::isArray(const json& j) {
    return j.is_array();
}

bool JsonLdUtils::isEmptyArray(const json& j) {
    return j.is_array() && j.empty();
}

bool JsonLdUtils::isArrayOfStrings(const json& j) {
    if(!j.is_array())
        return false;
    return std::all_of(j.cbegin(), j.cend(), [](const json &v){ return v.is_string(); });
}

bool JsonLdUtils::isScalar(const json& j) {
    return !j.is_null() && !j.is_array() && !j.is_object();
}

bool JsonLdUtils::isArrayOfScalars(const json& j) {
    if(!j.is_array())
        return false;
    return std::all_of(j.cbegin(), j.cend(), [](const json &v){ return isScalar(v); });
}

bool JsonLdUtils::deepContains(const json& values, const json& value) {
    return std::any_of(values.cbegin(), values.cend(), [&value](const json &v){ return deepCompare(v, value); });
}

/**
 * Merges value into the array at obj[key], if array at obj[key] does not yet contain it. If
 * array at obj[key] does not exist, create it and add value.
 *
 * @param obj the JSON "object"
 * @param key the key
 * @param value the value to store in array at obj[key]
 */
void JsonLdUtils::mergeValue(json & obj, const std::string& key, const json& value) {
    if (obj.is_null()) {
        return;
    }
    json & values = obj[key];
    if (values.is_null()) {
        values = json::array();
        obj[key] = values;
    }
    if (key == LIST || value.contains(LIST) || !deepContains(values, value)) {
        values.push_back(value);
    }
}

/**
 * Check if the given IRI ends with a URI general delimiter character.
 *
 * See https://tools.ietf.org/html/rfc3986#section-2.2
 *
 * @param iri the IRI to check
 * @return true if the IRI ends with a URI general delimiter character
 */
bool JsonLdUtils::iriEndsWithGeneralDelimiterCharacter(const std::string &iri) {
    if(iri.empty())
        return false;
    char c = iri[iri.size()-1];
    return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@';
}

/**
 * Add a value to an entry in a JSON object using a specified key.
 *
 * See: https://w3c.github.io/json-ld-api/#dfn-add-value
 *
 * @param object the object to add the value to
 * @param key the key
 * @param value the value
 * @param asArray if true, and value of key in the object doesn't exist or is not an array, set key to a new array containing the value
 */
void JsonLdUtils::addValue(json &object, const std::string & key, const json & value, bool asArray) {

    // 1)
    // If asArray is true and the value of key in object does not exist or is not an array, set it to a new array containing any original value.
    if(asArray) {
        if (!object.contains(key))
            object[key] = json::array();
        else {
            json origValue = object[key];
            if (!isArray(origValue)) {
                object[key] = json::array({origValue});
            }
        }
    }

    // 2)
    // If value is an array, then for each element v in value, use addValue() recursively to add v to key in entry.
    if(isArray(value)) {
        for(const auto& v: value)
            addValue(object, key, v, asArray);
    }

    // 3)
    // Otherwise:
    else {
        // 3.1)
        // If key is not an entry in object, add value as the value of key in object.
        if(!object.contains(key))
            object[key] = value;

        // 3.2)
        // Otherwise:
        else {
            // 3.2.1)
            // If the value of the key entry in object is not an array, set it to a new array containing the original value.
            json origValue = object[key];
            if (!isArray(origValue)) {
                object[key] = json::array({origValue});
            }

            // 3.2.2)
            // Append value to the value of the key entry in object.
            object[key].push_back(value);
        }
    }
}

/**
 * Returns whether a json object contains or equals a given string. We need this
 * function because the json library requires slightly different syntax if the json
 * object is an "object", "array" or "string".
 *
 * @param j the json object
 * @param value the value to compare with
 * @return true if the object contains or equals the given value
 */
bool JsonLdUtils::containsOrEquals(json & j, const std::string& value) {

    if(j.is_string())
        return j == value;

    else if(j.is_array()) {
        return std::any_of(j.cbegin(), j.cend(), [&value](const json &v){ return v == value; });
    }

    else if(j.is_object()) {
        for (const auto &e : j.items())
            if (e.key() == value)
                return true;
    }

    return false;
}
