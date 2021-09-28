#ifndef IMPL_REPORT_COMMANDRUNNER
#define IMPL_REPORT_COMMANDRUNNER

#include <string>
#include <array>
#include "CommandRunner.h"

class CommandRunner
{
    public:
        static constexpr int BUFFER_SIZE = 128;
        CommandRunner();
        CommandRunner(std::string);
        int start();
        int stop();
        std::string getNextOutput();
        void setCommand(std::string s){ command = s; };
    private:
        FILE* pipe;
        std::string command;
        std::array<char, BUFFER_SIZE> buffer;
};
#endif // IMPL_REPORT_COMMANDRUNNER
