#include "jsonld-cpp/FileLoader.h"
#include "jsonld-cpp/MediaType.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/JSONDocument.h"
#include "jsonld-cpp/RDFDocument.h"
#include <sstream>
#include <fstream>

using json = nlohmann::json;

namespace {

    MediaType detectContentType(const std::string& filename) {
        if(filename.empty())
            throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                              "filename is empty");

        const std::string nq = ".nq";
        if(filename.rfind(nq) == filename.length() - nq.length())
            return MediaType::n_quads();

        const std::string json = ".json";
        if(filename.rfind(json) == filename.length() - json.length())
            return MediaType::json();

        const std::string jsonld = ".jsonld";
        if(filename.rfind(jsonld) == filename.length() - jsonld.length())
            return MediaType::json_ld();

        const std::string html = ".html";
        if(filename.rfind(html) == filename.length() - html.length())
            return MediaType::html();

        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Unknown media type for file: " + filename);
    }

}


std::unique_ptr<RemoteDocument> FileLoader::loadDocument(const std::string &url) {

    // todo: check cache

    // check file properties
    if(url.find("file://") != 0 && url.find('/') != 0)
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Unsupported URL scheme. FileLoader only loads files.");

    std::string localUrl = url;

    if(localUrl.find("file://") == 0)
        localUrl = localUrl.substr(7);

    MediaType contentType = detectContentType(localUrl);

    // open stream

    std::ifstream inputStream{localUrl};
    if(!inputStream.is_open())
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Failed to open file: " + localUrl);

    // todo: add to cache

    if (JSONDocument::accepts(contentType)) {
        if(localUrl.find("file://") == std::string::npos)
            localUrl = "file://" + localUrl;

        return std::unique_ptr<RemoteDocument>(
                new JSONDocument(JSONDocument::of(contentType, inputStream, localUrl)));
    }

    if (RDFDocument::accepts(contentType)) {
        return std::unique_ptr<RemoteDocument>(
                new RDFDocument(RDFDocument::of(contentType, inputStream)));

    }

    std::stringstream ss;
    ss << "Unsupported content type: '" << contentType << "'. Supported content types are: ["
       << MediaType::json_ld() << ", " << MediaType::json() << " and " << MediaType::n_quads() << "]";
    throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());

}

FileLoader *FileLoader::clone() const {
    return new FileLoader(*this);
}
