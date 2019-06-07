#ifndef LIBJSONLD_CPP_TESTHELPERS_H
#define LIBJSONLD_CPP_TESTHELPERS_H


#include <string>
#include "json.hpp"


std::string resolvePath(const std::string &relPath);

std::string getTestNumberStr(int i);

std::string getBaseUri(std::string testNumber);

std::string getInputStr(std::string testNumber);

nlohmann::json getExpectedJson(std::string testNumber);


#endif //LIBJSONLD_CPP_TESTHELPERS_H
