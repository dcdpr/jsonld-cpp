#ifndef IMPL_REPORT_CONFIG
#define IMPL_REPORT_CONFIG

#include <string>
#include <set>
#include <vector>
#include "jsoninc.h"

class ConfigReader
{
    private:
        nlohmann::json document;

    public:
        ConfigReader(std::string filename);
        std::vector<std::vector<std::string>> getTestsuites();
        std::string getProject();
        std::string getMaker();
        nlohmann::json getSubjects();
};

#endif //IMPL_REPORT_CONFIG
