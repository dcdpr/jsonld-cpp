#ifndef LIBJSONLD_CPP_OBJUTILS_H
#define LIBJSONLD_CPP_OBJUTILS_H

#include "jsonld-cpp/jsoninc.h"

namespace ObjUtils {

    using json = nlohmann::json;

    // predicates for some JSON-LD object types
    // see https://www.w3.org/TR/json-ld11/#json-ld-specific-term-definitions

    bool isDefaultObject(const json& object);
    bool isNodeObject(const json& object);

}

#endif //LIBJSONLD_CPP_OBJUTILS_H
