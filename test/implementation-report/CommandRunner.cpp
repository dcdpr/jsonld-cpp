#include <iostream>
#include <cstdio>
#include "CommandRunner.h"

CommandRunner::CommandRunner(){}
CommandRunner::CommandRunner(std::string command) : command{command}{}

int CommandRunner::start()
{
    pipe = popen(command.c_str(), "r");
    if ( !pipe )
    {
        std::cerr << "Could not run command: " << command << std::endl;
        return 1;
    }
    return 0;
}

int CommandRunner::stop()
{
    std::cout << "Command completed" << std::endl;
    return pclose( pipe );
}

std::string CommandRunner::getNextOutput()
{
    if ( fgets( buffer.data(), BUFFER_SIZE, pipe ) == NULL )
    {
        return "";
    }
    return buffer.data();
}
