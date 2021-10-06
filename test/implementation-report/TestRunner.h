#ifndef IMPL_REPORT_TESTRUNNER
#define IMPL_REPORT_TESTRUNNER

#include <set>
#include <string>
#include <vector>
#include <set>
#include "TestResult.h"

class TestRunner
{
    private:
        std::string project;
        std::string user;
        std::string path;
        std::set<std::vector<std::string>> executables;
    public:
        TestRunner(std::string, std::string, std::string, std::set<std::vector<std::string>>);
        std::set<TestResult> run();
};
#endif // IMPL_REPORT_TESTRUNNER
