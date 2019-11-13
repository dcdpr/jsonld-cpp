#ifndef LIBJSONLD_CPP_OBJUTILS_H
#define LIBJSONLD_CPP_OBJUTILS_H

#include "jsoninc.h"

namespace ObjUtils {

    using json = nlohmann::json;

    json newMap();
    json newMap(const std::string& key, json value);
}

#endif //LIBJSONLD_CPP_OBJUTILS_H
