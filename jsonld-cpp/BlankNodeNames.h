#ifndef LIBJSONLD_CPP_BLANKNODENAMES_H
#define LIBJSONLD_CPP_BLANKNODENAMES_H

#include <string>
#include <map>
#include <vector>

/**
 * BlankNodeNames is a class that is used to generate new blank node identifiers or to
 * relabel an existing blank node identifier to avoid collision by the introduction of
 * new ones.
 *
 * By default, identifiers are generated using the prefix "_:b" with a number appended, for
 * example: "_:b0", "_:b1", ... but the prefix can be set in the constructor.
 *
 * See: https://www.w3.org/TR/json-ld11-api/#generate-blank-node-identifier
 */
class BlankNodeNames {
private:
    int counter = 0;
    std::string prefix = "_:b";
    std::map<std::string, std::string> keysToNames;
    std::vector<std::string> keysInInsertionOrder;

public:
    /**
     * Create new BlankNodeNames object
     */
    BlankNodeNames();

    /**
     * Create new BlankNodeNames object, using prefix as the prefix to create blank node names.
     *
     * Note: there is a convention that in JSON-LD, a blank node is assigned an identifier
     * starting with the prefix "_:". Any prefix given here should also start with _:, for
     * example: "_:dcd"
     *
     * @param prefix the prefix to use
     */
    explicit BlankNodeNames(std::string prefix);

    /**
     * Generates a blank node name for the given identifier using the algorithm
     * specified in:
     *
     * https://www.w3.org/TR/json-ld11-api/#generate-blank-node-identifier
     *
     * @return A blank node name.
     */
    std::string get();

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
    std::string get(const std::string & identifier);

    /**
     * Does key already exist as a name?
     *
     * @param key The key.
     * @return true if key exists as a name
     */
    bool exists(const std::string & key);

    /**
     * Gets collection of all existing keys
     *
     * @return All existing keys
     */
    std::vector<std::string> getKeys();

    /**
     * Checks if name has the form of a blank node name.
     *
     * @param name The name.
     * @return true if name has the form of a blank node name
     */
    static bool hasFormOfBlankNodeName(const std::string & name);

    bool operator==(const BlankNodeNames &rhs) const;

    bool operator!=(const BlankNodeNames &rhs) const;
};

#endif //LIBJSONLD_CPP_BLANKNODENAMES_H
