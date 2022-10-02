#ifndef LIBJSONLD_CPP_JSONLDURL_H
#define LIBJSONLD_CPP_JSONLDURL_H

#include <string>

struct JsonLdUrl {

    /**
     * Return URL which is the result of resolving 'pathToResolve' against 'baseUri'
     *
     * For more details about relative URI resolution, see https://www.rfc-editor.org/rfc/rfc3986#section-5
     *
     * Ex: if baseUri is "http://a/b/c/d;p?q" and pathToResolve is "../g", result will be "http://a/b/g"
     *
     * @param baseUri the base URI
     * @param pathToResolve the relative URI to resolve against base
     * @return the resolved URI. This may be empty if baseUri or pathToResolve are nullptr or empty.
     */
    static std::string resolve(const std::string * baseUri, const std::string * pathToResolve);

};

#endif //LIBJSONLD_CPP_JSONLDURL_H
