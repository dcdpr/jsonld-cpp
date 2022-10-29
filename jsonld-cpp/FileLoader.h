#ifndef LIBJSONLD_CPP_FILELOADER_H
#define LIBJSONLD_CPP_FILELOADER_H

#include "jsonld-cpp/DocumentLoader.h"

class RemoteDocument;

class FileLoader : public DocumentLoader {
public:

    FileLoader* clone() const override;

    // load url and return a RemoteDocument
    std::unique_ptr<RemoteDocument> loadDocument(const std::string &url) override;
};



#endif //LIBJSONLD_CPP_FILELOADER_H
