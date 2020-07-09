#ifndef LIBJSONLD_CPP_MANIFESTLOADER_H
#define LIBJSONLD_CPP_MANIFESTLOADER_H

#include "DocumentLoader.h"
#include "TestCase.h"
#include <string>
#include <map>

/**
 * Loads a test manifest file and returns collection of TestCases
 */
class ManifestLoader {
public:

    ManifestLoader(const std::string &manifestBase, const std::string &manifestName);

    std::map<std::string, TestCase> load();

private:
    DocumentLoader documentLoader;
    std::string manifestBase;
    std::string manifestName;
};


#endif //LIBJSONLD_CPP_MANIFESTLOADER_H
