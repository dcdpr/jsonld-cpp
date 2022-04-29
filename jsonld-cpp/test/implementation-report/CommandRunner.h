#ifndef IMPL_REPORT_COMMANDRUNNER
#define IMPL_REPORT_COMMANDRUNNER

#include <string>
#include <array>
#include "CommandRunner.h"
#include <sstream>

class CommandRunner
{
    public:
        static constexpr int BUFFER_SIZE = 128;
        CommandRunner();
        std::string run();
        void set_command(std::string s){ command = s; }
    private:
        std::string command;
        std::array<char, BUFFER_SIZE> buffer;
        std::stringstream output;
};
#endif // IMPL_REPORT_COMMANDRUNNER
