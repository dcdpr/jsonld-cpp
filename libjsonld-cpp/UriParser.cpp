#include "UriParser.h"

#include <cstring>
#include <uriparser/Uri.h>

class UriParserImpl {
public:
    UriParserImpl() {
        // Initialize the UriUriA struct this class wraps to a sane state.
        memset(static_cast<void *>(&uri_), 0, sizeof(UriUriA));
    }

    ~UriParserImpl() {
        uriFreeUriMembersA(&uri_);
    }

    UriUriA* get_mutable_uri() {
        return &uri_;
    }

    UriUriA* get_uri() const {
        return const_cast<UriUriA*>(&uri_);
    }

    // This helper function detects the existence of the given component and
    // converts it to a string if one is supplied.  If this component does not
    // exist false is returned.  If the component does exist true is returned.
    bool GetUriComponent(const UriTextRangeA& text_range,
                         std::string* output) const {
        if (!text_range.first || !text_range.afterLast) {
            return false;
        }
        if (output) {
            output->assign(text_range.first, text_range.afterLast - text_range.first);
        }
        return true;
    }

private:
    UriUriA uri_;
};


UriParser * UriParser::create(const char *str) {
    if(str == nullptr)
        return nullptr;
    auto* uri_parser = new UriParser;
    if (uri_parser->Parse(str)) {
        return uri_parser;
    }
    delete uri_parser;
    return nullptr;
}

UriParser * UriParser::createResolved(const char *base, const char *relative) {
    std::unique_ptr<UriParser> base_uri(create(base));
    std::unique_ptr<UriParser> relative_uri(create(relative));
    if (!base_uri || !relative_uri) {
        return nullptr;
    }
    auto* resolved_uri = new UriParser;
    if (resolved_uri->Resolve(*base_uri, *relative_uri)) {
        return resolved_uri;
    }
    delete resolved_uri;
    return nullptr;
}

UriParser::~UriParser() = default;

bool UriParser::Parse(const char *str) {
    UriParserStateA state;
    state.uri = pimpl_->get_mutable_uri();
    if (uriParseUriA(&state, str) != URI_SUCCESS) {
        uriFreeUriMembersA(pimpl_->get_mutable_uri());
        return false;
    }
    return true;
}

bool UriParser::Normalize() {
    return uriNormalizeSyntaxA(pimpl_->get_mutable_uri()) == URI_SUCCESS;
}

bool UriParser::Resolve(const UriParser &base, const UriParser &relative) {
    return uriAddBaseUriA(pimpl_->get_mutable_uri(),
                          relative.pimpl_->get_uri(),
                          base.pimpl_->get_uri()) == URI_SUCCESS;
}

bool UriParser::ToString(std::string* output) const {
    if (!output) {
        return false;
    }
    int chars_required;
    if (uriToStringCharsRequiredA(pimpl_->get_mutable_uri(),
                                  &chars_required) != URI_SUCCESS) {
        return false;
    }
    char* dest_str = (char*)malloc(chars_required+1);
    if (!dest_str) {
        return false;
    }
    int chars_written;
    if (uriToStringA(dest_str, pimpl_->get_mutable_uri(),
                     chars_required+1, &chars_written) != URI_SUCCESS) {
        free(dest_str);
        return false;
    }
    *output = dest_str;
    free(dest_str);
    return true;
}

bool UriParser::GetScheme(std::string *scheme) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->scheme, scheme);
}

bool UriParser::GetHost(std::string *host) const {
    return pimpl_->GetUriComponent(
            pimpl_->get_uri()->hostText, host);
}

bool UriParser::GetPort(std::string *port) const {
    return pimpl_->GetUriComponent(
            pimpl_->get_uri()->portText, port);
}

bool UriParser::GetQuery(std::string *query) const {
    return pimpl_->GetUriComponent(
            pimpl_->get_uri()->query, query);
}

bool UriParser::GetFragment(std::string *fragment) const {
    return pimpl_->GetUriComponent(
            pimpl_->get_uri()->fragment, fragment);
}

bool UriParser::GetPath(std::string *path) const {
    if (!pimpl_->get_uri()->pathHead ||
        !pimpl_->get_uri()->pathTail) {
        return false;
    }
    if (path) {
        path->clear();
        UriPathSegmentA* segment = pimpl_->get_uri()->pathHead;
        while (segment) {
            UriTextRangeA* text_range = &segment->text;
            if (!text_range || !text_range->first || !text_range->afterLast) {
                return false;  // Something is corrupt.
            }
            path->append(text_range->first,
                         text_range->afterLast - text_range->first);
            segment = segment->next;
            if (segment) {  // If there's a next segment append a separator.
                path->append("/");
            }
        }
    }
    return true;
}

UriParser::UriParser()
        : pimpl_(new UriParserImpl)
{
}
