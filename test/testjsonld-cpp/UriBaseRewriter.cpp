#include "UriBaseRewriter.h"

UriBaseRewriter::UriBaseRewriter(
        const std::string &isourceBase,
        const std::string &itargetBase,
        std::unique_ptr<DocumentLoader> iloader) :
        sourceBase(isourceBase),
        targetBase(itargetBase),
        loader(std::move(iloader)) {}

std::unique_ptr<RemoteDocument> UriBaseRewriter::loadDocument(const std::string &url) {

    if(url.find(sourceBase) != 0) {
        return loader->loadDocument(url); // todo: when might this happen?
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
