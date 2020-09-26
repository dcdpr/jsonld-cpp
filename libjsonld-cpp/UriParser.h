#ifndef LIBJSONLD_CPP_URIPARSER_H
#define LIBJSONLD_CPP_URIPARSER_H

#include <memory>
#include <string>

// A class that wraps "external/uriparser" library with a C++ API.

class UriParserImpl;

class UriParser {

public:
    // UriParser should always be constructed from one of the following static methods.

    // This creates a UriParser from a URI in string form.
    static UriParser* create(const char* str);

    // This creates a UriParser representing the resolution of the given
    // relative URI against the given base URI.
    static UriParser* createResolved(const char* base, const char* relative);

    // The destructor must perform uriparser-specific operations to release
    // resources.  It is highly recommdended that a UriParser* be managed
    // with boost::scoped_ptr or equivalent (as is done in CreateResolveUri).
    ~UriParser();

    // This parses the given URI string into the UriParser object and obliterates
    // any previous URI parsed into this object.  If the parse succeeds true is
    // returned, else false is returned.  This method is intended for use mainly
    // with the CreateFromParse() static method.
    bool Parse(const char* str);

    // UriParser (and the underlying uriparser library) does not automatically
    // normalize any URI.  (Normalize resolves the ..'s in a path, for example).
    // This method may be used at any time to normalize the URI.  RFC 3986
    // requires a fetching client to normalize a URI before fetching it.
    bool Normalize();

    // This resolves the URI represented by the UriParser relative against the
    // URI represented by the UriParser base.  This method is intended for use
    // mainly with the CreateResolvedUri() static method.
    bool Resolve(const UriParser& base, const UriParser& relative);

    // This method saves the URI in string form into the given string.  This
    // returns false if a NULL string argument is supplied or on any internal
    // errors in saving to this string.  True is returned on success.
    bool ToString(std::string* output) const;

    // This returns the scheme of the URI if one exists.
    bool GetScheme(std::string* scheme) const;

    // This returns the host of the URI if one exists.
    bool GetHost(std::string* host) const;

    // This returns the port of the URI if one exists.
    bool GetPort(std::string* port) const;

    // This returns the query of the URI if one exists.
    bool GetQuery(std::string* query) const;

    // This method returns the fragment portion of the URI into the given
    // string if such is supplied.  If no string is supplied or if there is no
    // fragment in this URI false is returned.  The fragment returned does not
    // include the '#' found in the corresponding URI.
    bool GetFragment(std::string* fragment) const;

    // This method returns true if the uri has a path.  If an output string
    // pointer is supplied the path is saved there.
    bool GetPath(std::string* path) const;

protected:
    UriParser();

private:
    // UriParserImpl hides "external/uriparser" types from clients
    const std::unique_ptr<UriParserImpl> pimpl_;

};


#endif //LIBJSONLD_CPP_URIPARSER_H
