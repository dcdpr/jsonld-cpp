#include <iostream>
#include "JsonLdUtils.h"
#include "JsonLdConsts.h"
#include "Uri.h"

bool JsonLdUtils::isKeyword(const std::string& property) {

    return
            property == "@base" ||
            property == "@context" ||
            property == "@container" ||
            property == "@default" ||
            property == "@embed" ||
            property == "@explicit" ||
            property == "@graph" ||
            property == "@id" ||
            property == "@index" ||
            property == "@language" ||
            property == "@list" ||
            property == "@omitDefault" ||
            property == "@reverse" ||
            property == "@preserve" ||
            property == "@set" ||
            property == "@type" ||
            property == "@value" ||
            property == "@vocab" ||
            property == "@requireAll";
}

bool JsonLdUtils::isAbsoluteIri(const std::string &iri) {
    std::cout << "isAbsoluteIri: " << iri
              << " " << (iri.find(':') != std::string::npos)
              << " " << Uri::isAbsolute(iri) << std::endl;
    return Uri::isAbsolute(iri);
}

bool JsonLdUtils::isRelativeIri(const std::string &iri) {
    return !(isKeyword(iri) || isAbsoluteIri(iri));
}

bool JsonLdUtils::deepCompare(JsonLdUtils::json v1, JsonLdUtils::json v2) {
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

bool JsonLdUtils::isList(const JsonLdUtils::json& j) {
    return j.contains(JsonLdConsts::LIST);
}

bool JsonLdUtils::isValue(const JsonLdUtils::json& j) {
    return j.contains(JsonLdConsts::VALUE);
}

bool JsonLdUtils::isObject(const JsonLdUtils::json& j) {
    return j.is_object();
}

bool JsonLdUtils::deepContains(const JsonLdUtils::json& values, const JsonLdUtils::json& value) {
    for (const auto& item : values) {
        if (deepCompare(item, value)) {
            return true;
        }
    }
    return false;
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
    if (key == "@list" || value.contains("@list") || !deepContains(values, value)) {
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
