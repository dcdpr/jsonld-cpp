#ifndef LIBJSONLD_CPP_OBJUTILS_H
#define LIBJSONLD_CPP_OBJUTILS_H

#include "jsonld-cpp/jsoninc.h"

namespace ObjUtils {

    using json = nlohmann::json;

    json newMap();
    json newMap(const std::string& key, json value);

    // predicates for JSON-LD object types
    // see https://www.w3.org/TR/json-ld11/#json-ld-specific-term-definitions

    bool isDefaultObject(json object);
    bool isNodeObject(json object);

}

#endif //LIBJSONLD_CPP_OBJUTILS_H
