#include "jsonld-cpp/detail/Uri.h"

#include "jsonld-cpp/detail/UriParser.h"


bool Uri::isUri(const std::string &uri) {
    if(uri.empty())
        return false;
    std::unique_ptr<UriParser> uri_parser(UriParser::create(uri.c_str()));
    return uri_parser.get();
}

bool Uri::isAbsolute(const std::string &uri) {
    if(uri.empty())
        return false;
    std::unique_ptr<UriParser> uri_parser(UriParser::create(uri.c_str()));
    return uri_parser.get() && uri_parser->GetScheme(nullptr);
}

bool Uri::NormalizeUri(const std::string &uri, std::string *result) {
    std::unique_ptr<UriParser> uri_parser(UriParser::create(uri.c_str()));
    return uri_parser.get() && uri_parser->Normalize() && uri_parser->ToString(result);
}

bool Uri::ResolveUri(const std::string &base, const std::string &relative, std::string *result) {
    std::unique_ptr<UriParser> uri_parser(UriParser::createResolved(base.c_str(), relative.c_str()));
    return uri_parser.get() && uri_parser->ToString(result);
}
