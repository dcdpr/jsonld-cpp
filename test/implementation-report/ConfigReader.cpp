#include "ConfigReader.h"

#include <iostream>
#include <fstream>
#include <iomanip>

ConfigReader::ConfigReader(std::string  filename)
{
    std::ifstream myfile ( filename );

    if  ( myfile.is_open() ){
        std::cout << "Reading config " << filename << std::endl;
        myfile >> document;
    } else {
        std::cout << "Could not read config " << filename << std::endl;
    }
}

std::set<std::vector<std::string>> ConfigReader::getTestsuites()
{
    std::set<std::vector<std::string>> testsuites;
    for ( const auto s : document["testsuites"])
    {
        std::vector<std::string> ts;
        ts.push_back(s["file"]);
        ts.push_back(s["manifest"]);
        testsuites.insert(ts);
    }
    return testsuites;
}

std::string ConfigReader::getProject()
{
    return "project";
}

std::string ConfigReader::getMaker()
{
    return "maker";
}

nlohmann::json ConfigReader::getSubjects()
{
    return document["header"]["subject"];
}
