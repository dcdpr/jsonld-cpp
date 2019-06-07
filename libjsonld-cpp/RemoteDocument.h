#ifndef JSONLD_CPP_REMOTEDOCUMENT_H
#define JSONLD_CPP_REMOTEDOCUMENT_H

#include <string>
#include "json.hpp"

class RemoteDocument {
private:
    std::string url;
    nlohmann::json document;

public:
    RemoteDocument(const std::string &url, const nlohmann::json &document);

    const std::string &getUrl() const;

    const nlohmann::json &getDocument() const;

};


#endif //JSONLD_CPP_REMOTEDOCUMENT_H
