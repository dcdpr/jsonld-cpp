#ifndef LIBJSONLD_CPP_URIBASEREWRITER_H
#define LIBJSONLD_CPP_URIBASEREWRITER_H


#include <DocumentLoader.h>
/**
 * Replaces the base part of a Uri before attempting the load a document. Used mainly for locating
 * test files locally.
 */
class UriBaseRewriter : public DocumentLoader {
public:

    UriBaseRewriter(const std::string &sourceBase, const std::string &targetBase,
                    std::unique_ptr<DocumentLoader> loader);

    UriBaseRewriter(const UriBaseRewriter & other);             // copy constructor
    UriBaseRewriter& operator=(const UriBaseRewriter & other);  // copy assignment

    std::unique_ptr<RemoteDocument> loadDocument(const std::string &url) override;

    UriBaseRewriter *clone() const override;

private:

    std::string sourceBase;
    std::string targetBase;
    std::unique_ptr<DocumentLoader> loader;
};


#endif //LIBJSONLD_CPP_URIBASEREWRITER_H
