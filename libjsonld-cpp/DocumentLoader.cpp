#include "DocumentLoader.h"
#include "json.hpp"
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <sstream>

using json = nlohmann::json;
using namespace boost::filesystem;

namespace {

    std::string getFileContents(const path &p) {
        ifstream f{p};
        const auto size = file_size(p);
        std::string buffer(size, ' ');
        f.read(&buffer[0], static_cast<std::streamsize>(size));
        return buffer;
    }

}


RemoteDocument DocumentLoader::loadDocument(const std::string &url) {

    std::cout << "load: " << cache.size() << std::endl;

    // first check the cache
    auto it = cache.find(url);
    if(it != cache.end())
        return RemoteDocument(url, it->second);

    // do something to load
    path p(url);
    if(exists(p)) {
        // read file into string
        std::string fileContents = getFileContents(p);

        // create the json object
        json j = json::parse(fileContents);

        // add to cache
        cache[url] = j;

        return RemoteDocument(url, j);
    }
    else {
        std::stringstream ss;
        ss << "Error: Url not found: [" << url << "]";
        throw std::runtime_error(ss.str());
    }

}

void DocumentLoader::addDocumentToCache(const std::string &url, const std::string &contents) {
    json j = json::parse(contents);
    cache.insert(std::make_pair(url, j));
}
