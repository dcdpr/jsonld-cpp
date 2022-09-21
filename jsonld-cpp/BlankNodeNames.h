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
    std::string get(const std::string & key);

    bool exists(const std::string & key);
    std::vector<std::string> getKeys();
};

#endif //LIBJSONLD_CPP_BLANKNODENAMES_H
