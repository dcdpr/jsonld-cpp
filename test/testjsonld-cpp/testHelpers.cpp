#include "testHelpers.h"
#include <boost/filesystem.hpp>
#include <sstream>
#include <iomanip>

// A somewhat "parent recursive" relative path file finder. If file is not found
// at current dir + relPath, it will go up to parent dir and try again. This is mainly
// for use by tests to find test data files in the main source hierarchy when being built
// by cmake in an out-of-source subdirectory.
std::string resolvePath(const std::string &relPath) {
    namespace fs = boost::filesystem;
    auto baseDir = fs::current_path();
    while (baseDir.has_parent_path())
    {
        auto combinePath = baseDir / relPath;
        if (fs::exists(combinePath))
        {
            return combinePath.string();
        }
        baseDir = baseDir.parent_path();
    }
    std::stringstream ss;
    ss << "Error: File at relative path [" << relPath << "] not found";
    throw std::runtime_error(ss.str());
}

std::string getTestNumberStr(int i) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << i;
    return ss.str();
}

std::string getTestNumberStr(const std::string& prefix, int i) {
    std::stringstream ss;
    ss << prefix << std::setw(3) << std::setfill('0') << i;
    return ss.str();
}

std::string getBaseUri(const std::string& testName, const std::string& testNumber) {
    return "https://w3c.github.io/json-ld-api/tests/" + testName + "/" + testNumber + "-in.jsonld";
}

std::string getInputStr(const std::string& testName, const std::string& testNumber) {
    std::ifstream fsIn {resolvePath("test/testjsonld-cpp/test_data/" + testName + "/" + testNumber + "-in.jsonld") };
    std::string inputStr {std::istreambuf_iterator<char>(fsIn), std::istreambuf_iterator<char>() };
    return inputStr;
}

nlohmann::json getExpectedJson(const std::string& testName, const std::string& testNumber) {
    std::ifstream fsOut {resolvePath("test/testjsonld-cpp/test_data/" + testName + "/" + testNumber + "-out.jsonld") };
    std::string outputStr { std::istreambuf_iterator<char>(fsOut), std::istreambuf_iterator<char>() };
    nlohmann::json expected = nlohmann::json::parse(outputStr);
    return expected;
}

std::string getExpectedRDF(const std::string& testName, const std::string& testNumber) {
    std::ifstream fsOut {resolvePath("test/testjsonld-cpp/test_data/" + testName + "/" + testNumber + "-out.nq") };
    std::string outputStr { std::istreambuf_iterator<char>(fsOut), std::istreambuf_iterator<char>() };
    return outputStr;
}
