#include "ConfigReader.h"

#include <iostream>
#include <fstream>

ConfigReader::ConfigReader(const std::string& filename)
{
    /**
     * The first thing that we do is take the filename passed as a parameter to
     * this method and initialise an input filestreeam.
     */
    std::ifstream myfile( filename );
    /**
     * We are expecting the file to contain JSON data and so can try and create
     * a JSON object from the content.  If it doesn't exist or contains invalid
     * JSON then we need to stop the program and give an indication as to what
     * the problem might be.
     */
    try {
        myfile >> document;
    } catch ( nlohmann::json::exception const&) {
        std::cout << "Could not read config " << filename
            << " - are you sure it exists and contains valid JSON?" << std::endl;
        exit( EXIT_FAILURE );
    }
}

std::vector<std::vector<std::string>> ConfigReader::getTestsuites()
{
    /**
     *  We want to transform the JSON data into a vector of string vectors so
     *  set up a foreach loop and build up the structure.
     */
    std::vector<std::vector<std::string>> testsuites;
    for ( const auto &s : document["testsuites"])
    {
        std::vector<std::string> ts;
        ts.push_back(s["file"].get<std::string>());
        ts.push_back(s["manifest"].get<std::string>());
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

nlohmann::json ConfigReader::getPrefixes() {
    nlohmann::json j;
    if ( document["header"].contains("prefix") )
    {
        j = document["header"]["prefix"];
    }
    return j;
}
