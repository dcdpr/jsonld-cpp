#ifndef LIBJSONLD_CPP_URIPARSER_H
#define LIBJSONLD_CPP_URIPARSER_H

#include <memory>
#include <string>

/**
 * UriParser wraps "external/uriparser" library with a C++ API.
 *
 * UriParser should most often be constructed from one of the static "create" methods.
 *
 * It is recommended that a UriParser* be managed with std::unique_ptr or equivalent.
 */

/**
 * Based on Google's UriParser class from KML 1.0 library
 */

class UriParserImpl;

class UriParser {

public:

    /**
     * Creates a UriParser from a char string
     *
     * @param uri URI string to parse
     * @return pointer to UriParser is successful, nullptr if not
     */
    static UriParser* create(const char* uri);

    /**
     * Creates a UriParser from the resolution of the relativeUri against the baseUri
     *
     * @param baseUri Base URI string
     * @param relativeUri Relative URI string to parse and resolve against baseUri
     * @return pointer to UriParser is successful, nullptr if not
     */
    static UriParser* createResolved(const char* baseUri, const char* relativeUri);

    ~UriParser();

    /**
     * Parses the URI char string and copies its component parts into this UriParser object
     *
     * Any previous URI parsed into this object is overwritten.
     *
     * @param uri URI string to parse
     * @return true if parsing is successful, false if not
     */
    bool Parse(const char* uri);

    /**
     * Normalize the URI stored in this UriParser object.
     *
     * For more details, see https://www.rfc-editor.org/rfc/rfc3986#section-6
     *
     * @return true if normalizing is successful, false if not
     */
    bool Normalize();

    /**
     * Set the URI stored in this UriParser object to the result of the resolution of
     * the UriParser 'relative' against the UriParser 'base'
     *
     * @param base Base URI
     * @param relative Relative URI to resolve against base
     * @return true if resolution is successful, false if not
     */
    bool Resolve(const UriParser& base, const UriParser& relative);

    /**
     * Copy URI to output string.
     *
     * @param output string
     * @return true if successful, false if error or output string is nullptr
     */
    bool ToString(std::string* output) const;

    /**
     * Copy URI's scheme to output string
     *
     * @param output string
     * @return true if scheme exists and output string is not nullptr
     */
    bool GetScheme(std::string* output) const;

    /**
     * Copy URI's host to output string
     *
     * @param output string
     * @return true if host exists and output string is not nullptr
     */
    bool GetHost(std::string* output) const;

    /**
     * Copy URI's port to output string
     *
     * @param output string
     * @return true if port exists and output string is not nullptr
     */
    bool GetPort(std::string* output) const;

    /**
     * Copy URI's query to output string
     *
     * @param output string
     * @return true if query exists and output string is not nullptr
     */
    bool GetQuery(std::string* output) const;

    /**
     * Copy URI's fragment to output string
     *
     * The fragment returned does not include the '#'.
     *
     * @param output string
     * @return true if fragment exists and output string is not nullptr
     */
    bool GetFragment(std::string* output) const;

    /**
     * Copy URI's path to output string
     *
     * @param output string
     * @return true if path exists and output string is not nullptr
     */
    bool GetPath(std::string* output) const;

protected:
    UriParser();

private:
    const std::unique_ptr<UriParserImpl> pimpl_;

};


#endif //LIBJSONLD_CPP_URIPARSER_H
