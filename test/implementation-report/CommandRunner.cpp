#include <iostream>
#include <cstdio>
#include "CommandRunner.h"

CommandRunner::CommandRunner(){}

std::string CommandRunner::run()
{
    FILE* pipe;
    // clear any error states on the stringstream
    output.clear();
    // now clear the contents of the stringstream to reuse it
    output.str(std::string());

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
