#ifndef LIBJSONLD_CPP_TESTCASE_H
#define LIBJSONLD_CPP_TESTCASE_H

#include "jsoninc.h"

#include <string>
#include <set>

struct TestCase {

    static TestCase create(nlohmann::json o, const std::string& manifestUri, const std::string& manifestBase, const std::string& baseUri);

    std::string id;

    std::string name;

    std::string input;

    std::string context;

    std::string expect;

    std::string frame;

//public JsonLdErrorCode expectErrorCode;

    std::string baseUri;

    std::string uri;

    std::set<std::string> type;

//public JsonLdTestCaseOptions options;

};


#endif //LIBJSONLD_CPP_TESTCASE_H
