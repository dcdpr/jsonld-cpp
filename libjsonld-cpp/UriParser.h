#ifndef LIBJSONLD_CPP_URIPARSER_H
#define LIBJSONLD_CPP_URIPARSER_H

#include <memory>
#include <string>

/**
 * UriParser wraps "external/uriparser" library with a C++ API.
 *
 * UriParser should most often be constructed from one of the static "create" methods.
 *
 * It is recommended that a UriParser* be managed with stdd::unique_ptr or equivalent.
 *
 */
class UriParserImpl;

class UriParser {

public:

    // This creates a UriParser from a URI.
    static UriParser* create(const char* str);

    // This creates a UriParser representing the resolution of the
    // relative URI against the base URI.
    static UriParser* createResolved(const char* base, const char* relative);

    ~UriParser();

    // This parses the given URI string into the UriParser object and obliterates
    // any previous URI parsed into this object.  If the parse succeeds true is
    // returned, else false is returned.  This method is intended for use mainly
    // with the create() static method.
    bool Parse(const char* str);

    // UriParser (and the underlying uriparser library) does not automatically
    // normalize any URI.  (Normalize resolves the ..'s in a path, for example).
    // This method may be used at any time to normalize the URI.  RFC 3986
    // requires a fetching client to normalize a URI before fetching it.
    bool Normalize();

    // This resolves the URI represented by the UriParser relative against the
    // URI represented by the UriParser base.  This method is intended for use
    // mainly with the CreateResolved() static method.
    bool Resolve(const UriParser& base, const UriParser& relative);

    // This method outputs the URI in string form.  This returns false if
    // a NULL string argument is supplied or on any internal errors in saving
    // to this string.  True is returned on success.
    bool ToString(std::string* output) const;

    /**
     * Copy URI's scheme to output string if the scheme exists and output string is not null
     *
     * @param output string
     * @return true if scheme exists
     */
    bool GetScheme(std::string* output) const;

    /**
     * Copy URI's host to output string if the host exists and output string is not null
     *
     * @param output string
     * @return true if host exists
     */
    bool GetHost(std::string* output) const;

    /**
     * Copy URI's port to output string if the port exists and output string is not null
     *
     * @param output string
     * @return true if port exists
     */
    bool GetPort(std::string* output) const;

    /**
     * Copy URI's query to output string if the query exists and output string is not null
     *
     * @param output string
     * @return true if query exists
     */
    bool GetQuery(std::string* output) const;

    /**
     * Copy URI's fragment to output string if the fragment exists and output string is not null
     *
     * The fragment returned does not include the '#'.
     *
     * @param output string
     * @return true if fragment exists
     */
    bool GetFragment(std::string* output) const;

    /**
     * Copy URI's path to output string if the path exists and output string is not null
     *
     * @param output string
     * @return true if path exists
     */
    bool GetPath(std::string* output) const;

protected:
    UriParser();

private:
    // UriParserImpl hides "external/uriparser" types from clients
    const std::unique_ptr<UriParserImpl> pimpl_;

};


#endif //LIBJSONLD_CPP_URIPARSER_H
