#include "RemoteDocument.h"

RemoteDocument::RemoteDocument(const std::string &url, const nlohmann::json &document)
: url(url), document(document)
{}

const std::string &RemoteDocument::getUrl() const {
    return url;
}

const nlohmann::json &RemoteDocument::getDocument() const {
    return document;
}


