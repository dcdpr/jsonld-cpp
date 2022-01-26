#include "ConfigReader.h"

#include <iostream>
#include <fstream>
#include <iomanip>

ConfigReader::ConfigReader(std::string  filename)
{
    std::ifstream myfile( filename );

    if  ( myfile.is_open() ){
        myfile >> document;
    } else {
        std::cout << "Could not read config " << filename << std::endl;
    }
}

std::vector<std::vector<std::string>> ConfigReader::getTestsuites()
{
    std::vector<std::vector<std::string>> testsuites;
    for ( const auto s : document["testsuites"])
    {
        std::vector<std::string> ts;
        ts.push_back(s["file"]);
        ts.push_back(s["manifest"]);
        testsuites.push_back(ts);
    }
    return testsuites;
}

nlohmann::json ConfigReader::getSubjects()
{
    nlohmann::json j;
    if ( document["header"].contains("subject") )
    {
        j = document["header"]["subject"];
    }
    return j;
}
