#ifndef LIBJSONLD_CPP_FILELOADER_H
#define LIBJSONLD_CPP_FILELOADER_H

#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/DocumentLoader.h"

class FileLoader : public DocumentLoader {
private:
// todo: trying to create this cache object is giving me "copy constructor is implicitly deleted because 'unique_ptr<RemoteDocument,
//      std::__1::default_delete<RemoteDocument> >' has a user-declared move constructor" errors
//    std::map<std::string, std::unique_ptr<RemoteDocument>> cache;

public:

    virtual FileLoader* clone() const override;

    //void addDocumentToCache(const std::string &url, const std::string &contents);

    // load url and return a RemoteDocument
    std::unique_ptr<RemoteDocument> loadDocument(const std::string &url) override;
};



#endif //LIBJSONLD_CPP_FILELOADER_H
