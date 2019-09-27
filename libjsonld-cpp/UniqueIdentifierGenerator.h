#ifndef LIBJSONLD_CPP_UNIQUEIDENTIFIERGENERATOR_H
#define LIBJSONLD_CPP_UNIQUEIDENTIFIERGENERATOR_H


#include <string>
#include <map>

class UniqueIdentifierGenerator {
private:
    int counter = 0;
    std::string prefix = "_:b";
    std::map<std::string, std::string> existingIdentifiers;
    std::vector<std::string> identifiersInsertionOrder;

public:
    UniqueIdentifierGenerator();
    explicit UniqueIdentifierGenerator(std::string prefix);

    std::string generate();
    std::string generate(const std::string & id);

    bool exists(const std::string & id);
    std::vector<std::string> getIdentifiers();
    std::vector<std::string> getIdentifiersInsertionOrder();

    std::string toString();
};


#endif //LIBJSONLD_CPP_UNIQUEIDENTIFIERGENERATOR_H
