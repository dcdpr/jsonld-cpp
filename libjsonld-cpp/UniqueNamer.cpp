#include "UniqueNamer.h"
#include <sstream>
#include <utility>
#include <vector>

UniqueNamer::UniqueNamer() = default;

UniqueNamer::UniqueNamer(std::string iprefix)
        : prefix(std::move(iprefix)) { }

/**
 * Generates a blank node name for the given key using the algorithm
 * specified in:
 *
 * http://www.w3.org/TR/json-ld-api/#generate-blank-node-identifier
 *
 * @return A fresh, unused, blank node name.
 */
std::string UniqueNamer::get() {
    std::stringstream ss;
    ss << prefix << counter++;
    return ss.str();
}

/**
 * Generates a blank node name for the given key using the algorithm
 * specified in:
 *
 * http://www.w3.org/TR/json-ld-api/#generate-blank-node-identifier
 *
 * @param key
 *            The key.
 * @return A blank node name based on key.
 */
std::string UniqueNamer::get(const std::string & key) {
    if(keysToNames.count(key))
        return keysToNames.at(key);
    std::string name = get();
    keysToNames[key] = name;
    keysInInsertionOrder.push_back(key);
    return name;
}

/**
 * Gets collection of all existing keys
 *
 * @return All existing keys
 */
std::vector<std::string> UniqueNamer::getKeys() {
    return keysInInsertionOrder;
}

/**
 * Does key already exist as a name?
 *
 * @param key The key.
 * @return true if key exists as a name
 */
bool UniqueNamer::exists(const std::string &key) {
    return keysToNames.count(key) > 0;
}
