#include "JsonLdUrl.h"
#include "IriUtils.h"

std::string JsonLdUrl::resolve(std::string *baseUri, std::string *pathToResolve) {
    if (baseUri == nullptr) {
        return *pathToResolve;
    }
    if (pathToResolve == nullptr || pathToResolve->empty()) {
        return *baseUri;
    }
    return IriUtils::prependBase(*baseUri, *pathToResolve);
}
