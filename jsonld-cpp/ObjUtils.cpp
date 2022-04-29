#include "jsonld-cpp/ObjUtils.h"
#include "jsonld-cpp/JsonLdConsts.h"

using json = nlohmann::json;

json ObjUtils::newMap() {
    return json::object();
}

json ObjUtils::newMap(const std::string& key, json value) {
    auto result = newMap();
    result[key] = value;
    return result;
}

/**
 * Check if a json object is a default object
 *
 * See: https://www.w3.org/TR/json-ld11/#dfn-default-object
 *
 * @param object the object to test
 * @return true if object is a default object
 */
bool ObjUtils::isDefaultObject(json object) {
    return object.is_object() && object.contains(JsonLdConsts::DEFAULT);
}


/**
 * Check if a json object is a node object
 *
 * See: https://www.w3.org/TR/json-ld11/#dfn-node-object
 *
 * @param object the object to test
 * @return true if object is a node object
 */
bool ObjUtils::isNodeObject(json object) {
    return object.is_object() &&
           ((!object.contains(JsonLdConsts::VALUE) &&
             !object.contains(JsonLdConsts::LIST) &&
             !object.contains(JsonLdConsts::SET)) ||
            (object.contains(JsonLdConsts::CONTEXT) &&
             object.contains(JsonLdConsts::GRAPH))
           );
}


