#include <iostream>
#include <sstream>
#include <regex>
#include <ctime>
#include "TestRunner.h"
#include "RdfData.h"

#include <typeinfo>

TestRunner::TestRunner(
        std::string project,
        std::string user,
        std::string path,
        std::vector<std::vector<std::string>> exe)
    : project{project}, user{user}, path{path}, executables{exe}
{
//    std::cout << "Creating TestRunner" << std::endl;
    executable_iterator = 0;
    auto command = executables.at(executable_iterator);
    std::string fullpath ( path + command.at(0) );
    manifest = command.at(1);
    cr.set_command( fullpath );
    command_output = cr.run();
    ss << command_output;
}

void TestRunner::start()
{
//    std::cout << "TestRunner::start" << std::endl;
    has_next_output = true;
}

bool TestRunner::next_executable()
{
    has_next_output = false;
//    std::cout << "executable: " << executable_iterator << " of " << executables.size() << std::endl;
    if ( executable_iterator < executables.size() )
    {
        auto command = executables.at(executable_iterator);
        std::string fullpath ( path + command.at(0) );
        manifest = command.at(1);
        cr.set_command( fullpath );
        command_output = cr.run();
        ss << command_output;
        ss.clear();
        ss.seekg(0);
        has_next_output = true;
        executable_iterator++;
        return true;
    }
    return false;
}

TestResult TestRunner::next_result()
{
//    std::cout << "TestRunner::next_result" << std::endl;
    // prepare result
    TestResult tr;
    if ( ! has_next_output )
    {
        std::cerr << "No next output!" << std::endl;
        return tr;
    }
    find_next_result();
    // get pass/fail
    std::smatch result;
    std::regex_search( next_output, result, std::regex( "([A-Z]+)") );
    std::smatch id;
    std::regex_search( next_output, id, std::regex( "[A-Za-z0-9]*(?= \\()") );
    // create the TestResult object
    tr.manifest = manifest;
    tr.test = id[0];
    tr.result = result[0];
    tr.time = time(0);
    // std::cout << tr << std::endl;
    return tr;
}

void TestRunner::find_next_result()
{
//    std::cout << "TestRunner::find_next_result" << std::endl;

    std::string line;
    while ( std::getline( ss, line, '\n' ) )
    {
        // find the next result output
        if ( ! line.find("[  ",0) )
        {
            next_output = line;
            return;
        }
    }
    if ( next_executable() ) find_next_result();
}

bool TestRunner::has_next()
{
//    std::cout << "TestRunner::has_next" << std::endl;
    return has_next_output;
}
