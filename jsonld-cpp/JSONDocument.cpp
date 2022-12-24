#include "jsonld-cpp/JSONDocument.h"
#include "jsonld-cpp/JsonLdError.h"
#include <fstream>
#include <utility>
#include <sstream>

JSONDocument::JSONDocument(MediaType icontentType, nlohmann::json idocument, std::string idocumentUrl)
        : contentType(std::move(icontentType)), documentUrl(std::move(idocumentUrl)), document(std::move(idocument))
{
}

MediaType JSONDocument::getContentType() const {
    return contentType;
}

const std::string &JSONDocument::getContextUrl() const {
    return contextUrl;
}

const RDF::RDFDataset & JSONDocument::getRDFContent() const {
    // todo: could return std::optional when using C++17
    throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                      "No RDF content for this JSONDocument");
}

const std::string &JSONDocument::getDocumentUrl() const {
    return documentUrl;
}

const nlohmann::json &JSONDocument::getJSONContent() const {
    return document;
}

JSONDocument JSONDocument::of(const MediaType& contentType, std::istream &in, const std::string & documentUrl) {
    assertContentType(contentType);

    if(!in.good())
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Failed to create JSONDocument.");
    std::string fileData {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>() };
    nlohmann::json jsonData = nlohmann::json::parse(fileData);
    return {contentType, jsonData, documentUrl};
}

bool JSONDocument::accepts(const MediaType& contentType) {
    return MediaType::json_ld() == contentType
            || MediaType::json() == contentType;
}

void JSONDocument::assertContentType(const MediaType& contentType) {
    if(!accepts(contentType)) {
        std::stringstream ss;
        ss << "Unsupported content type: '" << contentType << "'.";
        throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
    }
}

void JSONDocument::setContextUrl(const std::string &url) {
    contextUrl = url;
}

void JSONDocument::setDocumentUrl(const std::string &url) {
    documentUrl = url;
}
