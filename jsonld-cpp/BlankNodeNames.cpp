#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include <sstream>
#include <utility>
#include <vector>

BlankNodeNames::BlankNodeNames() = default;

BlankNodeNames::BlankNodeNames(std::string prefix)
        : prefix(std::move(prefix)) { }

std::string BlankNodeNames::get() {
    std::stringstream ss;
    ss << prefix << counter++;
    return ss.str();
}

std::string BlankNodeNames::get(const std::string & identifier) {
    if(keysToNames.count(identifier))
        return keysToNames.at(identifier);

    std::string name = get();
    keysToNames[identifier] = name;
    keysInInsertionOrder.push_back(identifier);
    return name;
}

std::vector<std::string> BlankNodeNames::getKeys() {
    return keysInInsertionOrder;
}

bool BlankNodeNames::exists(const std::string &key) {
    return keysToNames.count(key) > 0;
}

bool BlankNodeNames::hasFormOfBlankNodeName(const std::string &name) {
    return name.find(JsonLdConsts::BLANK_NODE_PREFIX) == 0;
}

bool BlankNodeNames::operator==(const BlankNodeNames &rhs) const {
    return counter == rhs.counter &&
           prefix == rhs.prefix &&
           keysToNames == rhs.keysToNames &&
           keysInInsertionOrder == rhs.keysInInsertionOrder;
}

bool BlankNodeNames::operator!=(const BlankNodeNames &rhs) const {
    return !(rhs == *this);
}
