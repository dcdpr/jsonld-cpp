#ifndef LIBJSONLD_CPP_BLANKNODEIDENTIFIERGENERATOR_H
#define LIBJSONLD_CPP_BLANKNODEIDENTIFIERGENERATOR_H


#include <string>
#include <map>

class BlankNodeIdentifierGenerator {
private:
    int blankNodeCounter = 0;
    std::map<std::string, std::string> blankNodeIdentifierMap;

public:
    BlankNodeIdentifierGenerator();
    std::string generate();
    std::string generate(const std::string & id);
};


#endif //LIBJSONLD_CPP_BLANKNODEIDENTIFIERGENERATOR_H
