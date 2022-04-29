#ifndef IMLP_REPORT_TESTRESULT
#define IMLP_REPORT_TESTRESULT

#include <string>
#include <sstream>
#include <ctime>

struct TestResult
{
    std::string manifest;
    std::string test;
    std::string result;
    std::time_t time;

    bool empty() const {
        return manifest.empty() && test.empty() && result.empty();
    }

    // overload the equality operator
    friend int operator== ( const TestResult& lhs, const TestResult& rhs)
    {
        // can't have two prefixes that are the same
        return ( lhs.manifest == rhs.manifest && lhs.test == rhs.test );
    }
    // overload the greater than operator
    friend int operator< ( const TestResult& lhs, const TestResult& rhs)
    {
        return ( lhs.manifest < rhs.manifest && lhs.test < rhs.test );
    }
    // overload the output operator
    friend std::ostream& operator<< ( std::ostream& os, const TestResult& t)
    {
        os << "TEST RESULT: "
            << " manifest: " << t.manifest
            << " test: " << t.test
            << " result: " << t.result
            << " time: " << t.time;
        return os;
    }
};

#endif //IMLP_REPORT_TESTRESULT
