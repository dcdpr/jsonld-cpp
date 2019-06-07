#include <iostream>
#include "JsonLdUrl.h"

std::string JsonLdUrl::resolve(std::string *baseUri, std::string *pathToResolve) {
    if (baseUri == nullptr) {
        return *pathToResolve;
    }
    if (pathToResolve == nullptr || *pathToResolve == "") { // todo: was == pathToResolve.trim()
        return *baseUri;
    }
    std::cout << "JsonLdUrl::resolve: " << *baseUri << " " << *pathToResolve << std::endl;
    return IriUtils::prependBase(*baseUri, *pathToResolve);
}
