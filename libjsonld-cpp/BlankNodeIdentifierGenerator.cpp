#include <sstream>
#include <iostream>
#include "BlankNodeIdentifierGenerator.h"

BlankNodeIdentifierGenerator::BlankNodeIdentifierGenerator() = default;

/**
 * Generates a blank node identifier for the given key using the algorithm
 * specified in:
 *
 * http://www.w3.org/TR/json-ld-api/#generate-blank-node-identifier
 *
 * @return A fresh, unused, blank node identifier.
 */
std::string BlankNodeIdentifierGenerator::generate() {
    std::stringstream ss;
    ss << "_:b" << blankNodeCounter++;
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
std::string BlankNodeIdentifierGenerator::generate(const std::string & id) {
    if(blankNodeIdentifierMap.count(id))
        return blankNodeIdentifierMap.at(id);
    std::string bnid = generate();
    blankNodeIdentifierMap[id] = bnid;
    return bnid;
}
