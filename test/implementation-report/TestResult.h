#ifndef IMLP_REPORT_TESTRESULT
#define IMLP_REPORT_TESTRESULT

struct TestResult
{
    std::string manifest;
    std::string test;
    std::string result;

    // overlad the equality operator
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
};

#endif //IMLP_REPORT_TESTRESULT
