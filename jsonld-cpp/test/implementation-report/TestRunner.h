#ifndef IMPL_REPORT_TESTRUNNER
#define IMPL_REPORT_TESTRUNNER

#include <set>
#include <string>
#include <vector>
#include <set>
#include "TestResult.h"
#include "CommandRunner.h"
#include <sstream>

/**
 * \brief   The interface that handles running the Test Executables
 *
 * \details The TestRunner class simplified the interface for running one or
 *          more testsuites and returning the results.
 */
class TestRunner
{
    private:
        /**
         * We need to store the path to the executables and so we provide
         * this private member to do that which is populated by the constructor
         */
        std::string path;

        /**
         * The second argument passed to the constructor is a vector of string
         * vectors which provide the name of the test executable and the name
         * of the associated manifest (only used in writing the report).
         */
        std::vector<std::vector<std::string>> executables;

        /**
         * The current manifest name is extracted for each executable and stored
         * so that it can be used when generating the TestResult() instances.
         */
        std::string manifest;

        /**
         * To keep a track of which value in the executables vector we are
         * currently processng we have this property.
         */
        size_t executable_iterator;

        /**
         * A CommandRunner() is used to handle the envoking of an executable
         * from the vector
         */
        CommandRunner cr;

        /**
         * The command output is buffered in the ss
         */
        std::stringstream ss;
        bool has_next_output;
        //helper methods
        bool next_executable();
        std::string find_next_result();

    public:
        // constructor
        TestRunner( std::string, std::vector<std::vector<std::string>> );
        // control methods
        void start();
        TestResult next_result();
        bool has_next() const;
};
#endif // IMPL_REPORT_TESTRUNNER
