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
            // this is another keyword found reading through the api spec. Not sure why they aren't
            // included in the above.
      ANY
    };
}

bool JsonLdUtils::isKeyword(const std::string& str) {
    return knownKeywords.count(str) > 0;
}

bool JsonLdUtils::isKeywordForm(const std::string& property) {
    std::regex keywordForm(R"(@[A-Za-z]+)");
    std::smatch match;
    return std::regex_match(property, match, keywordForm);
}

bool JsonLdUtils::isAbsoluteIri(const std::string &str) {
    return Uri::isAbsolute(str);
}

bool JsonLdUtils::isRelativeIri(const std::string &str) {
    return !(isKeyword(str) || isAbsoluteIri(str));
}

bool JsonLdUtils::isIri(const std::string &str) {
    return !isKeyword(str) && Uri::isUri(str);
}

bool JsonLdUtils::deepCompare(json j1, json j2) {
    if (j1.is_null())
        return j2.is_null();

    else if (j2.is_null())
        return j1.is_null();

    else if (j1.is_object() && j2.is_object()) {
        if (j1.size() != j2.size()) {
            return false;
        }
        for (auto &el: j1.items()) {
            if (!j2.contains(el.key()) || !deepCompare(el.value(), j2.at(el.key())))
                return false;
        }
        return true;
    }

    else if (j1.is_array() && j2.is_array()) {
        if (j1.size() != j2.size()) {
            return false;
        }
        // used to mark members of j2 that we have already matched to avoid
        // matching the same item twice for lists that have duplicates
        std::vector<bool> alreadyMatched(j2.size());
        for (const auto &o1: j1) {
            bool gotmatch = false;

            for (size_t j = 0; j < j2.size(); j++) {
                if (!alreadyMatched[j] && deepCompare(o1, j2.at(j))) {
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
    }

    else
        return j1 == j2;

}

bool JsonLdUtils::isListObject(const json& j) {
    // A list object is a map that has a @list key. It may also have an @index
    // key, but no other entries.
    return JsonLdUtils::isObject(j) &&
           j.contains(LIST) &&
           (j.size() == 1 || (j.size() == 2 && j.contains(INDEX)));
}

bool JsonLdUtils::isGraphObject(const json& j) {
    // A graph object represents a named graph as the value of a map entry within a
    // node object. When expanded, a graph object must have an @graph entry, and
    // may also have @id, and @index entries.
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
    // A value object is a map that has an @value entry.
    return JsonLdUtils::isObject(j) && j.contains(VALUE);
}

bool JsonLdUtils::isObject(const json& j) {
    return j.is_object();
}

bool JsonLdUtils::isEmptyObject(const json& j) {
    return j.is_object() && j.empty();
}

bool JsonLdUtils::isDefaultObject(const json& j) {
    return j.is_object() && j.contains(DEFAULT);
}

bool JsonLdUtils::isNodeObject(const json& j) {
    // A map is a node object if it exists outside of the JSON-LD context and:
    //    * it does not contain the @value, @list, or @set keywords, or
    //    * it is not the top-most map in the JSON-LD document consisting of no other
    //    entries than @graph and @context.
    return j.is_object() &&
           ((!j.contains(VALUE) &&
             !j.contains(LIST) &&
             !j.contains(SET)) ||
            (j.contains(GRAPH) &&
             j.contains(CONTEXT))
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

bool JsonLdUtils::iriEndsWithGeneralDelimiterCharacter(const std::string &iri) {
    if(iri.empty())
        return false;
    char c = iri[iri.size()-1];
    return c == ':' || c == '/' || c == '?' || c == '#' || c == '[' || c == ']' || c == '@';
}

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
