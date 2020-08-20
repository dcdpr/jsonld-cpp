#include <fstream>
#include <utility>
#include <sstream>
#include "RDFDocument.h"
#include "JsonLdError.h"

RDFDocument::RDFDocument(MediaType icontentType, std::string idocument)
        : contentType(std::move(icontentType)), document(std::move(idocument))
{
}

MediaType RDFDocument::getContentType() const {
    return contentType;
}

const std::string &RDFDocument::getContextUrl() const {
    return contextUrl;
}

const std::string & RDFDocument::getRDFContent() const {
    return document;
}

const std::string &RDFDocument::getDocumentUrl() const {
    return documentUrl;
}

const nlohmann::json &RDFDocument::getJSONContent() const {
    // todo: could return std::optional when using C++17
    throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                      "No JSON content for this RDFDocument");
}

RDFDocument RDFDocument::of(const MediaType& contentType, std::istream &in) {
    assertContentType(contentType);

    if(!in.good())
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Failed to create RDFDocument.");

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line, '\n'))
        lines.push_back(line);
    std::sort(lines.begin(), lines.end());
    std::stringstream result;
    copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(result, "\n"));
    return RDFDocument(contentType, result.str());
}

bool RDFDocument::accepts(const MediaType& contentType) {
    return MediaType::n_quads() == contentType;
}

void RDFDocument::assertContentType(const MediaType& contentType) {
    if(!accepts(contentType)) {
        std::stringstream ss;
        ss << "Unsupported content type: '" << contentType << "'.";
        throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
    }
}

void RDFDocument::setContextUrl(const std::string &url) {
    contextUrl = url;
}

void RDFDocument::setDocumentUrl(const std::string &url) {
    documentUrl = url;
}