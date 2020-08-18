#include "FileLoader.h"
#include "MediaType.h"
#include "JsonLdError.h"
#include "JSONDocument.h"
#include "RDFDocument.h"
#include <iostream>
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

    // check cache

    // check file properties
    if(url.find("file") != 0 && url.find("/") != 0)
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Unsupported URL scheme. FileLoader only loads files.");

    // todo: going to need to strip "file://" and possibly deal with relative paths?

    MediaType contentType = detectContentType(url);

    // open stream

    std::ifstream inputStream{url};
    if(!inputStream.is_open())
        throw JsonLdError(JsonLdError::LoadingDocumentFailed,
                          "Failed to open file: " + url);

    // create document

    // add to cache

    // return

    if (JSONDocument::accepts(contentType)) {
        return std::unique_ptr<RemoteDocument>(
                new JSONDocument(JSONDocument::of(contentType, inputStream)));
    }

    if (RDFDocument::accepts(contentType)) {
        return std::unique_ptr<RemoteDocument>(
                new RDFDocument(RDFDocument::of(contentType, inputStream)));

    }


    // first check the cache
//    auto it = cache.find(url);
//    if(it != cache.end())
//        return RemoteDocument(url, it->second);

    // do something to load
    //path p(url);
//    if(exists(p)) {
//        MediaType mediaType = detectContentType(url);

        // read file into string
//        std::string fileContents = getFileContents2(url);
// todo: now need to make a jsondocument and rdfdocuemtns and have this function return one or the other
// depending on the mediatype
        // create the json object
//        json j = json::parse(fileContents);

        // add to cache
//        cache[url] = j;

//        return RemoteDocument(url, j);
//    }
//    else {
//        std::stringstream ss;
//        ss << "Error: Url not found: [" << url << "]";
//        throw std::runtime_error(ss.str());
//        // todo: detect and throw for common file errors (not readable, not found, etc)
//    }

    std::stringstream ss;
    ss << "Unsupported content type: '" << contentType << "'. Supported content types are: ["
       << MediaType::json_ld() << ", " << MediaType::json() << " and " << MediaType::n_quads();
    throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());

}

//void FileLoader::addDocumentToCache(const std::string &url, const std::string &contents) {
//    json j = json::parse(contents);
//    cache.insert(std::make_pair(url, j));
//}

FileLoader *FileLoader::clone() const {
    return new FileLoader(*this);
}
