#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include "TestRunner.h"
#include "CommandRunner.h"
#include "EarlFormatter.h"

TestRunner::TestRunner(
        std::string project,
        std::string user,
        std::string path,
        std::set<std::vector<std::string>> exe)
    : project{project}, user{user}, path{path}, executables{exe}{}
void TestRunner::run()
{
    CommandRunner cr;
    EarlFormatter ef;
    
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

                std::stringstream ss;
                ss << "https://w3c.github.io/json-ld-api/tests/"
                   << exe.at(1) << "_manifest#" << id[0];
                std::string test(ss.str());
                std::string passed(result[0]);

                std::cout << ef.assertion( project, user, test, passed ) << std::endl;
            };
        } while (o != "");
    }
}
