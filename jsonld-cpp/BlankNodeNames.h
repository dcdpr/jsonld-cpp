#ifndef LIBJSONLD_CPP_BLANKNODENAMES_H
#define LIBJSONLD_CPP_BLANKNODENAMES_H

#include <string>
#include <map>
#include <vector>


class BlankNodeNames {
private:
    int counter = 0;
    std::string prefix = "_:b";
    std::map<std::string, std::string> keysToNames;
    std::vector<std::string> keysInInsertionOrder;

public:
    BlankNodeNames();
    explicit BlankNodeNames(std::string prefix);

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

    static bool isBlankNodeName(const std::string & name);

};

#endif //LIBJSONLD_CPP_BLANKNODENAMES_H
