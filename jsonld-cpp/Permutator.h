#ifndef LIBJSONLD_CPP_PERMUTATOR_H
#define LIBJSONLD_CPP_PERMUTATOR_H

#include <vector>
#include <map>
#include <string>

// Thanks to jsonld-java for this class
// todo: make sure to copy license nd any header info

class Permutator {
private:
    std::vector<std::string> strings;
    std::map<std::string, bool> left;
    bool done;

public:
    explicit Permutator(std::vector <std::string> strings);
    bool hasNext() const;
    std::vector<std::string> next();
};

#endif //LIBJSONLD_CPP_PERMUTATOR_H
