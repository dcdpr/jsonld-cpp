#ifndef LIBJSONLD_CPP_MANIFESTLOADER_H
#define LIBJSONLD_CPP_MANIFESTLOADER_H

#include <string>
#include <map>

#include <jsonld-cpp/FileLoader.h>

#include "TestCase.h"


/**
 * Loads a test manifest file and returns collection of TestCases
 *
 * This class is slightly different than ManifestLoader for dealing with
 * the "URDNA" canonicalize test case manifests
 */
class UrdnaManifestLoader {
public:

    UrdnaManifestLoader(const std::string &imanifestBase, const std::string &imanifestName);

    std::map<std::string, TestCase> load();

private:
    FileLoader documentLoader;
    std::string manifestBase;
    std::string manifestName;
};


#endif //LIBJSONLD_CPP_MANIFESTLOADER_H
