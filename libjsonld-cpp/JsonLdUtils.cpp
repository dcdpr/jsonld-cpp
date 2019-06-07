#include "JsonLdUtils.h"

bool JsonLdUtils::isKeyword(std::string property) {

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

bool JsonLdUtils::isAbsoluteIri(const std::string &value) {
    // TODO: this is a bit simplistic!
    return value.find(":") != std::string::npos;
}

bool JsonLdUtils::isRelativeIri(const std::string &value) {
    return !(isKeyword(value) || isAbsoluteIri(value));
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
        for (int i = 0; i < v1.size(); i++) {
            json o1 = v1.at(i);
            bool gotmatch = false;

            for (int j = 0; j < v2.size(); j++) {
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
