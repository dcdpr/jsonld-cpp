#include <sstream>
#include <iostream>
#include <utility>
#include <vector>
#include "UniqueIdentifierGenerator.h"

//UniqueIdentifierGenerator::UniqueIdentifierGenerator() = default;
UniqueIdentifierGenerator::UniqueIdentifierGenerator() {std::cout << "Creating new UniqueIdentifierGenerator" << std::endl; }

UniqueIdentifierGenerator::UniqueIdentifierGenerator(std::string prefix)
: prefix(std::move(prefix)) { std::cout << "Creating new UniqueIdentifierGenerator, prefix: " << this->prefix << std::endl;}

/**
 * Generates a blank node identifier for the given key using the algorithm
 * specified in:
 *
 * http://www.w3.org/TR/json-ld-api/#generate-blank-node-identifier
 *
 * @return A fresh, unused, blank node identifier.
 */
std::string UniqueIdentifierGenerator::generate() {
    std::stringstream ss;
    ss << prefix << counter++;
    std::cout << "UniqueIdentifierGenerator counter++ : " << counter << std::endl;
    return ss.str();
}

/**
 * Generates a blank node identifier for the given key using the algorithm
 * specified in:
 *
 * http://www.w3.org/TR/json-ld-api/#generate-blank-node-identifier
 *
 * @param id
 *            The id.
 * @return A blank node identifier based on id.
 */
std::string UniqueIdentifierGenerator::generate(const std::string & id) {
    if(existingIdentifiers.count(id))
        return existingIdentifiers.at(id);
    std::string identifier = generate();
    existingIdentifiers[id] = identifier;
    identifiersInsertionOrder.push_back(id);
    return identifier;
}

/**
 * Gets collection of all existing identifiers
 *
 * @return All existing identifiers
 */
std::vector<std::string> UniqueIdentifierGenerator::getIdentifiers() {
    std::vector<std::string> v;
    v.reserve(existingIdentifiers.size());
    for(auto const & i : existingIdentifiers) {
        v.push_back(i.first);
    }
    return v;
}

std::vector<std::string> UniqueIdentifierGenerator::getIdentifiersInsertionOrder() {
    return identifiersInsertionOrder;
}

/**
 * Does id already exist as an identifier?
 *
 * @param id The id.
 * @return true if id exists as an identifier
 */
bool UniqueIdentifierGenerator::exists(const std::string &id) {
    return existingIdentifiers.count(id) > 0;
}

std::string UniqueIdentifierGenerator::toString() {
    std::stringstream ss;
    ss << "UniqueNamer{";
    ss << "prefix='" << prefix << "'";
    ss << ", counter=" << counter;
    ss << ", existing=";
    for(const auto& e : existingIdentifiers) {
        ss << e.first << "=" << e.second << ", ";
    }
    ss << "}";
    return ss.str();
}

