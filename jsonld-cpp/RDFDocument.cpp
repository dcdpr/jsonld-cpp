#include "jsonld-cpp/RDFDocument.h"

#include <iterator>
#include <sstream>
#include <utility>

#include "jsonld-cpp/NQuadsSerialization.h"
#include "jsonld-cpp/JsonLdError.h"


RDFDocument::RDFDocument(MediaType contentType, RDF::RDFDataset document)
        : contentType(std::move(contentType)), document(std::move(document))
{
}

MediaType RDFDocument::getContentType() const {
    return contentType;
}

const std::string &RDFDocument::getContextUrl() const {
    return contextUrl;
}

const RDF::RDFDataset & RDFDocument::getRDFContent() const {
    return document;
}

const std::string &RDFDocument::getDocumentUrl() const {
    return documentUrl;
}

const nlohmann::ordered_json &RDFDocument::getJSONContent() const {
    throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                      "No JSON content for this RDFDocument");
}

RDFDocument RDFDocument::of(const MediaType& contentType, std::istream &in) {
    assertContentType(contentType);

    if(!in.good())
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Failed to create RDFDocument.");

    // read data from incoming stream to a tmp string
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line, '\n'))
        lines.push_back(line);
    std::sort(lines.begin(), lines.end());
    std::stringstream result;
    std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(result, "\n"));
    // parse tmp string into an RDFDataset
    RDF::RDFDataset dataset = NQuadsSerialization::parse(result.str());
    return {contentType, dataset};
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
