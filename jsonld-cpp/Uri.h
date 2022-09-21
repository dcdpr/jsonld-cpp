#ifndef LIBJSONLD_CPP_URI_H
#define LIBJSONLD_CPP_URI_H


#include <string>

/**
 * Functions to help deal with URI strings.
 */

namespace Uri {

    /**
     * Tests if the string 'uri' is a URI or not
     *
     * @param uri the string to test
     * @return true if 'uri' is a URI, false if not
     */
    bool isUri(const std::string& uri);

    /**
     * Tests if the string 'uri' is an absolute URI or not
     *
     * An absolute URI is a URI that has a 'scheme' component.
     * Ex: "https://json-ld.org/", not "json-ld.org/"
     *
     * @param uri the string to test
     * @return true if 'uri' is an absolute URI, false if not
     */
    bool isAbsolute(const std::string& uri);

    /**
     * Normalize the string 'uri' and put the result in 'result'
     *
     * Normalizing a URI generally simplifies and cleans it up. For more
     * details, see https://www.rfc-editor.org/rfc/rfc3986#section-6
     *
     * Ex: if uri is "/a/b/c/./../../g", result will be "/a/g"
     *
     * @param uri the URI to normalize
     * @param result the normalized uri
     * @return true on success, false if not
     */
    bool NormalizeUri(const std::string& uri, std::string* result);

    /**
     * Set 'result' to the resolution of the URI 'relative' against the URI 'base'
     *
     * For more details about relative URI resolution, see https://www.rfc-editor.org/rfc/rfc3986#section-5
     *
     * Ex: if base is "http://a/b/c/d;p?q" and relative is "../g", result will be "http://a/b/g"
     *
     * @param base the base URI
     * @param relative the relative URI to resolve against base
     * @param result the resolved URI
     * @return true on success, false if not
     */
    bool ResolveUri(const std::string& base, const std::string& relative, std::string* result);

}


#endif //LIBJSONLD_CPP_URI_H
