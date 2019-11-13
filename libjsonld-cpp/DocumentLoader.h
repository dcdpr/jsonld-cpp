#ifndef LIBBECH32_DOCUMENTLOADER_H
#define LIBBECH32_DOCUMENTLOADER_H

#include "RemoteDocument.h"

class DocumentLoader {

    using json = nlohmann::json;

private:
    std::map<std::string, json> cache;

public:

    void addDocumentToCache(const std::string &url, const std::string &contents);

    // load url and return a RemoteDocument
    RemoteDocument loadDocument(const std::string &url);
};



#endif //LIBBECH32_DOCUMENTLOADER_H
