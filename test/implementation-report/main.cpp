#include <string>
#include <fstream>
#include <iostream>

#include "ReportWriter.h"
#include "TestRunner.h"
#include "ConfigReader.h"
#include "EarlFormatter.h"
#include "RdfDataBuilder.h"

std::string usage(std::string name)
{
    std::string s = "Usage\n:";
    s += name;
    s += " /path/to/configuration /path/to/executables \n\
         The configuration file should be in JSON format and created with \n\
         the configuration.html form \n\
         The implementation report will be created using the executables \n\
         defined in the JSON configuration file which should be located in \n\
         the path/to/executables location";
    return s; 
}

int main(int argc, char **argv)
{
    if ( argc < 2 )
    {
        std::cerr << usage(argv[0]) << std::endl;
        return 1;
    };
    // Initialize the report writer
    ReportWriter rw = ReportWriter( std::cout );

    // parse the configuration file
    ConfigReader cr ( argv[1] );
    
    // create the output formatter
    EarlFormatter ef;

    // create RdfDataBuilder
    RdfDataBuilder db;

    // parse the configuration header
    for ( auto j : cr.getSubjects() )
    {
        auto d = db.parse( j );
        ef.addRdfData( d );
    }

    // run the testsuites
    TestRunner tr (
            cr.getProject(),
            cr.getMaker(),
            argv[2],
            cr.getTestsuites());
    auto results = tr.run();

    // parse the results
    for (auto r : results )
    {
        auto rdf = db.parse( r );
        // add results to formatter
        ef.addRdfData( rdf );
    }

    // output the results
    rw.write ( ef.str() );
    return 0;
}

