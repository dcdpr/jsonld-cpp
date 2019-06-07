#ifndef LIBJSONLD_CPP_JSONLDURL_H
#define LIBJSONLD_CPP_JSONLDURL_H


#include <string>
#include "uri.hpp"
#include "IriUtils.h"

class JsonLdUrl {

public:
    static std::string resolve(std::string * baseUri, std::string * pathToResolve);

};


#endif //LIBJSONLD_CPP_JSONLDURL_H
