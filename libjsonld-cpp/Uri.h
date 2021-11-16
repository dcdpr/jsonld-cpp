#ifndef LIBJSONLD_CPP_URI_H
#define LIBJSONLD_CPP_URI_H


#include <string>

namespace Uri {

    bool ResolveUri(const std::string& base, const std::string& relative, std::string* result);

    bool isUri(const std::string& uri);
    bool isAbsolute(const std::string& uri);

    bool NormalizeUri(const std::string& uri, std::string* result);
}


#endif //LIBJSONLD_CPP_URI_H
