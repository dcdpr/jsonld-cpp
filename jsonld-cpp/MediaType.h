#ifndef LIBJSONLD_CPP_MEDIATYPE_H
#define LIBJSONLD_CPP_MEDIATYPE_H

#include <string>
#include <ostream>

class MediaType {

public:

    static MediaType of(const std::string &type, const std::string &subType);

    static MediaType html() { return of("text", "html"); }
    static MediaType json() { return of("application", "json"); }
    static MediaType json_ld() { return of("application", "ld+json"); }
    static MediaType n_quads() { return of("application", "n-quads"); }
    static MediaType any() { return of("*", "*"); }

    bool operator==(const MediaType &rhs) const;
    bool operator!=(const MediaType &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const MediaType &type);

private:

    std::string type;
    std::string subType;

    MediaType(std::string type, std::string subType);
};


#endif //LIBJSONLD_CPP_MEDIATYPE_H
