#include <iostream>
#include <sstream>
#include <regex>
#include "TestRunner.h"
#include "CommandRunner.h"
#include "RdfData.h"

TestRunner::TestRunner(
        std::string project,
        std::string user,
        std::string path,
        std::set<std::vector<std::string>> exe)
    : project{project}, user{user}, path{path}, executables{exe}{}

std::set<TestResult> TestRunner::run()
{
    CommandRunner cr;
    std::set<TestResult> resultset;

    for(auto it = executables.begin(); it != executables.end(); ++it)
    {
        auto exe = *it;
        std::string fullpath ( path + exe.at(0) );
        cr.setCommand(fullpath);
        cr.start();
        std::string o;
        do
        {
            o = cr.getNextOutput();
            if ( o.rfind("[   ",0 ) == 0 )
            {
                // get pass/fail
                std::smatch result;
                std::regex_search( o, result, std::regex( "([A-Z]+)") );
                std::smatch id;
                std::regex_search( o, id, std::regex( "[A-Za-z0-9]*(?= \\()") );
                // create the TestResult object
                TestResult tr;
                tr.manifest = exe.at(1);
                tr.test = id[0];
                tr.result = result[0];
                resultset.insert( tr );
            };
        } while (o != "");
    }
    return resultset;
}
