#ifndef LIBJSONLD_CPP_FILELOADER_H
#define LIBJSONLD_CPP_FILELOADER_H

#include "RemoteDocument.h"
#include "DocumentLoader.h"

class FileLoader : public DocumentLoader {

    using json = nlohmann::json;

private:
    std::map<std::string, json> cache;

public:

    virtual FileLoader* clone() const;

    void addDocumentToCache(const std::string &url, const std::string &contents);

    // load url and return a RemoteDocument
    RemoteDocument loadDocument(const std::string &url);
};



#endif //LIBJSONLD_CPP_FILELOADER_H
