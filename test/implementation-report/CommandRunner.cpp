#include <iostream>
#include <cstdio>
#include "CommandRunner.h"

CommandRunner::CommandRunner(){}
CommandRunner::CommandRunner(std::string command) : command{command}{}

std::string CommandRunner::run()
{
    FILE* pipe;
    pipe = popen( command.c_str(), "r" );
    if ( ! pipe )
    {
        std::cerr << "Could not run command: " << command << std::endl;
        return "";
    }
    while ( fgets( buffer.data(), BUFFER_SIZE, pipe ) )
    {
        output << buffer.data();
    }
    pclose( pipe );
    return output.str();
}
