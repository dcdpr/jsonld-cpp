#ifndef LIBJSONLD_CPP_MEDIATYPE_H
#define LIBJSONLD_CPP_MEDIATYPE_H


#include <string>
#include <ostream>

class MediaType {

public:

    static MediaType of(const std::string &type, const std::string &subType);

    static MediaType html() { return of(TYPE_TEXT, "html"); }
    static MediaType json() { return of(TYPE_APPLICATION, "json"); }
    static MediaType json_ld() { return of(TYPE_APPLICATION, "ld+json"); }
    static MediaType xhtml() { return of(TYPE_APPLICATION, "xhtml+xml"); }
    static MediaType n_quads() { return of(TYPE_APPLICATION, "n-quads"); }
    static MediaType any() { return of(WILDCARD, WILDCARD); }

    bool operator==(const MediaType &rhs) const;
    bool operator!=(const MediaType &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const MediaType &type);

private:
    static constexpr const char TYPE_TEXT[] = "text";
    static constexpr const char TYPE_APPLICATION[] = "application";
    static constexpr const char WILDCARD[] = "*";

    std::string type;
    std::string subType;

    MediaType(const std::string &itype, const std::string &isubType);
};


#endif //LIBJSONLD_CPP_MEDIATYPE_H
