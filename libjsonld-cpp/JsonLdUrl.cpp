#include "JsonLdUrl.h"
#include "Uri.h"

std::string JsonLdUrl::resolve(std::string *baseUri, std::string *pathToResolve) {
    if (baseUri == nullptr) {
        return *pathToResolve;
    }
    if (pathToResolve == nullptr || pathToResolve->empty()) {
        return *baseUri;
    }
    std::string result;
    if(Uri::ResolveUri(*baseUri, *pathToResolve, &result))
        return result;
    else
        return ""; // todo: throw?
}
