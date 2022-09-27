#include "jsonld-cpp/BlankNodeNames.h"
#include <sstream>
#include <utility>
#include <vector>

BlankNodeNames::BlankNodeNames() = default;

BlankNodeNames::BlankNodeNames(std::string iprefix)
        : prefix(std::move(iprefix)) { }

std::string BlankNodeNames::get() {
    std::stringstream ss;
    ss << prefix << counter++;
    return ss.str();
}

/**
 * Generates a blank node name for the given identifier using the algorithm
 * specified in:
 *
 * https://www.w3.org/TR/json-ld11-api/#generate-blank-node-identifier
 *
 * @param identifier
 *            The identifier.
 * @return A blank node name based on identifier.
 */
std::string BlankNodeNames::get(const std::string & identifier) {
    if(keysToNames.count(identifier))
        return keysToNames.at(identifier);
    std::string name = get();
    keysToNames[identifier] = name;
    keysInInsertionOrder.push_back(identifier);
    return name;
}

/**
 * Gets collection of all existing keys
 *
 * @return All existing keys
 */
std::vector<std::string> BlankNodeNames::getKeys() {
    return keysInInsertionOrder;
}

/**
 * Does key already exist as a name?
 *
 * @param key The key.
 * @return true if key exists as a name
 */
bool BlankNodeNames::exists(const std::string &key) {
    return keysToNames.count(key) > 0;
}
