#ifndef LIBJSONLD_CPP_TESTCASE_H
#define LIBJSONLD_CPP_TESTCASE_H

#include <string>
#include <set>

#include <jsonld-cpp/jsoninc.h>
#include <jsonld-cpp/JsonLdOptions.h>

#include "TestCaseOptions.h"


class TestCase {
public:
    explicit TestCase(const std::string &testsBase);

    static TestCase create(nlohmann::json manifestEntry, const std::string& manifestUri, const std::string& manifestBase, const std::string& baseUri);

    enum class HashAlgorithm { sha256, sha384 };

    std::string id;

    std::string name;

    std::string input;

    std::string context;

    std::string expect;

    std::string frame;

    std::string expectErrorCode;

    std::string baseUri;

    std::string uri;

    std::set<std::string> type;

    std::string testsBase;

    HashAlgorithm hashAlgorithm;

    TestCaseOptions options;

    JsonLdOptions getOptions();

};


#endif //LIBJSONLD_CPP_TESTCASE_H
