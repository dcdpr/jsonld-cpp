#ifndef IMPL_REPORT_FOLDERSCANNER
#define IMPL_REPORT_FOLDERSCANNER

#include <set>
#include <string>
#include "CommandRunner.h"

class FolderScanner
{
    private:
        std::string path;
        CommandRunner command;
        std::set<std::string> filenames;
    public:
        FolderScanner(std::string);
        void scan();
        std::set<std::string> getFileNames()
        {
            return filenames;
        }
};
#endif // IMPL_REPORT_FOLDERSCANNER
