#ifndef LIBJSONLD_CPP_DOCUMENTLOADER_H
#define LIBJSONLD_CPP_DOCUMENTLOADER_H

#include <string>
#include <memory>

class RemoteDocument;


class DocumentLoader {
public:
    DocumentLoader() = default;
    DocumentLoader(const DocumentLoader&) = default;
    DocumentLoader& operator=(const DocumentLoader&) = default;

    // virtual destructor needed as this is an abstract base class
    virtual ~DocumentLoader();

    // virtual clone function needed so that classes holding unique_ptrs to
    // DocumentLoader can be copied
    virtual DocumentLoader* clone() const = 0;

    // loads the document referred to by the url
    virtual std::unique_ptr<RemoteDocument> loadDocument(const std::string &url) = 0;

};

#endif //LIBJSONLD_CPP_DOCUMENTLOADER_H
