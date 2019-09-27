#ifndef LIBJSONLD_CPP_PERMUTATOR_H
#define LIBJSONLD_CPP_PERMUTATOR_H

#include <vector>
#include <map>
#include <string>

class Permutator {
private:
    std::vector<std::string> strings;
    bool done;
    std::map<std::string, bool> left;

public:
    explicit Permutator(std::vector <std::string> strings);
    bool hasNext();
    std::vector<std::string> next();
};

#endif //LIBJSONLD_CPP_PERMUTATOR_H
