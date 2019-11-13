#ifndef LIBJSONLD_CPP_UNIQUENAMER_H
#define LIBJSONLD_CPP_UNIQUENAMER_H

#include <string>
#include <map>
#include <vector>

/**
 * A UniqueNamer issues unique names, keeping track of any previously issued names.
 */
class UniqueNamer {
private:
    int counter = 0;
    std::string prefix = "_:b";
    std::map<std::string, std::string> keysToNames;
    std::vector<std::string> keysInInsertionOrder;

public:
    UniqueNamer();
    explicit UniqueNamer(std::string prefix);

    std::string get();
    std::string get(const std::string & key);

    bool exists(const std::string & key);
    std::vector<std::string> getKeys();
};

#endif //LIBJSONLD_CPP_UNIQUENAMER_H
