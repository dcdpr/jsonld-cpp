#include "UriBaseRewriter.h"

#include <jsonld-cpp/RemoteDocument.h>

#include <utility>


UriBaseRewriter::UriBaseRewriter(
        std::string sourceBase,
        std::string targetBase,
        std::unique_ptr<DocumentLoader> loader) :
        sourceBase(std::move(sourceBase)),
        targetBase(std::move(targetBase)),
        loader(std::move(loader)) {}

std::unique_ptr<RemoteDocument> UriBaseRewriter::loadDocument(const std::string &url) {

    if(url.find(sourceBase) != 0) {
        return loader->loadDocument(url);
    }

    std::string relativePath = url.substr(sourceBase.length());

    auto remoteDocument = loader->loadDocument(targetBase + relativePath);

    return remoteDocument;
}

UriBaseRewriter *UriBaseRewriter::clone() const {
    return new UriBaseRewriter(*this);
}

UriBaseRewriter::UriBaseRewriter(const UriBaseRewriter & other) {
    sourceBase = other.sourceBase;
    targetBase = other.targetBase;
    if(other.loader)
        loader.reset( other.loader->clone() );
}

UriBaseRewriter &UriBaseRewriter::operator=(const UriBaseRewriter & other) {
    sourceBase = other.sourceBase;
    targetBase = other.targetBase;
    if(other.loader)
        loader.reset( other.loader->clone() );
    return *this;
}
