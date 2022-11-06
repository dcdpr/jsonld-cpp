#include "jsonld-cpp/UriParser.h"
#include <uriparser/Uri.h>

class UriParserImpl {
public:
    UriParserImpl() = default;

    ~UriParserImpl() {
        uriFreeUriMembersA(&uri_);
    }

    UriUriA* get_mutable_uri() {
        return &uri_;
    }

    UriUriA* get_uri() const {
        return const_cast<UriUriA*>(&uri_);
    }

    static bool GetUriComponent(const UriTextRangeA& text_range,
                                std::string* output) {
        if (!text_range.first || !text_range.afterLast) {
            return false;
        }
        if (output) {
            output->assign(text_range.first, text_range.afterLast - text_range.first);
        }
        return true;
    }

private:
    UriUriA uri_{};
};


UriParser * UriParser::create(const char *uri) {
    if(uri == nullptr)
        return nullptr;
    auto* uri_parser = new UriParser;
    if (uri_parser && uri_parser->Parse(uri)) {
        return uri_parser;
    }
    delete uri_parser;
    return nullptr;
}

UriParser * UriParser::createResolved(const char *baseUri, const char *relativeUri) {
    std::unique_ptr<UriParser> base_uri(create(baseUri));
    std::unique_ptr<UriParser> relative_uri(create(relativeUri));
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

bool UriParser::Parse(const char *uri) {
    UriParserStateA state;
    state.uri = pimpl_->get_mutable_uri();
    if (uriParseUriA(&state, uri) != URI_SUCCESS) {
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
    char* dest_str = static_cast<char *>(malloc(chars_required + 1));
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

bool UriParser::GetScheme(std::string *output) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->scheme, output);
}

bool UriParser::GetHost(std::string *output) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->hostText, output);
}

bool UriParser::GetPort(std::string *output) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->portText, output);
}

bool UriParser::GetQuery(std::string *output) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->query, output);
}

bool UriParser::GetFragment(std::string *output) const {
    return pimpl_->GetUriComponent(pimpl_->get_uri()->fragment, output);
}

bool UriParser::GetPath(std::string *output) const {
    if (!pimpl_->get_uri()->pathHead ||
        !pimpl_->get_uri()->pathTail) {
        return false;
    }
    if (output) {
        output->clear();
        UriPathSegmentA* segment = pimpl_->get_uri()->pathHead;
        while (segment) {
            UriTextRangeA* text_range = &segment->text;
            if (!text_range || !text_range->first || !text_range->afterLast) {
                return false;
            }
            output->append(text_range->first,
                         text_range->afterLast - text_range->first);
            segment = segment->next;
            if (segment) {
                output->append("/");
            }
        }
    }
    return true;
}

UriParser::UriParser()
        : pimpl_(new UriParserImpl)
{
}
