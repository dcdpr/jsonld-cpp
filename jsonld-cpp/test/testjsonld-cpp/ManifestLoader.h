#ifndef LIBJSONLD_CPP_MANIFESTLOADER_H
#define LIBJSONLD_CPP_MANIFESTLOADER_H

#include <jsonld-cpp/FileLoader.h>
#include "TestCase.h"
#include <string>
#include <map>

/**
 * Loads a test manifest file and returns collection of TestCases
 */
class ManifestLoader {
public:

    ManifestLoader(const std::string &imanifestBase, const std::string &imanifestName);

    std::map<std::string, TestCase> load();

private:
    FileLoader documentLoader;
    std::string manifestBase;
    std::string manifestName;
};


#endif //LIBJSONLD_CPP_MANIFESTLOADER_H
