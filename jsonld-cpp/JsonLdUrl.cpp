#include "jsonld-cpp/JsonLdUrl.h"
#include "jsonld-cpp/detail/Uri.h"

std::string JsonLdUrl::resolve(const std::string *baseUri, const std::string *pathToResolve) {
    if (baseUri == nullptr) {
        if(pathToResolve == nullptr)
            return "";
        return *pathToResolve;
    }
    if (pathToResolve == nullptr || pathToResolve->empty()) {
        return *baseUri;
    }
    std::string result;
    if(Uri::ResolveUri(*baseUri, *pathToResolve, &result))
        return result;
    else
        return "";
}
