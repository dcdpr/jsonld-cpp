#include <iostream>
#include <cstdio>
#include "FolderScanner.h"

FolderScanner::FolderScanner(std::string path): path{path}{
    // find executable files in the filepath
    std::string s = "find " + path + " -type f -executable";
    command = CommandRunner(s);
}

void FolderScanner::scan()
{
    std::cout << "Scanning " << path << "..." << std::endl;
    command.start();
    std::string line = command.getNextOutput();
    int maxfiles = 100;

    while( line != "" || --maxfiles < 0 )
    {
        line.pop_back();                //remove newline character
        filenames.insert(line);         //insert into set
        line = command.getNextOutput(); //get the next line from command output
    }
    command.stop();
}
