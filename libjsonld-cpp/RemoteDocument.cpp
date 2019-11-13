#include "RemoteDocument.h"

#include <utility>

RemoteDocument::RemoteDocument(std::string iurl, nlohmann::json idocument)
: url(std::move(iurl)), document(std::move(idocument))
{}

const std::string &RemoteDocument::getUrl() const {
    return url;
}

const nlohmann::json &RemoteDocument::getDocument() const {
    return document;
}


