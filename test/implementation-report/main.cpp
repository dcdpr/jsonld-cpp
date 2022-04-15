#include <string>
#include <fstream>
#include <iostream>

#include "ReportWriter.h"
#include "TestRunner.h"
#include "ConfigReader.h"
#include "EarlFormatter.h"
#include "RdfDataBuilder.h"

/**
 * Formats and returns a string with program usage directions
 */
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

/**
* \mainpage Implementation Report
*
* \section intro_sec Introduction
* The Implementation Report was created for the jsonld-cpp project, to generate
* a report that could be submitted to the JSONLD working group and have the
* project recognised as an implementation of the spec.  As a great deal of the
* jsonld-cpp implementation had already been done by Dan Pape, the goal of the
* implementation report program was to take the existing testsuite output and
* convert that into the report.  In taking this approach, we would not have to
* make any changes to the existing jsonld-cpp implementation and could treat
* the report generation as a standalone sub-project.
*
* \section config_sec Configuration
* The implementation report requires some information not found in the gtest
* result data so we need to provide it to the program using a configuration file
* that contains the information in JSON.
*
* To help create the JSON configuration file we have a HTML page that can be
* used to input the information and generate the file in the required format.
* This can be achieved by opening the <a href="../../configuration.html"
* target="_blank">configuration.html</a> file that is in the
* implementation-report folder in a browser and you will see that there are a
* number of fields that contain default information that can be changed if
* needed.  Scrolling down past the initial fields, you will see a section named
* \e Developers where you can add one or more developers that work on the
* project.  There are different roles that the developers can be associated with
* that are defined in the \e doap context defined in the prefix. Clicking the \e
* Delete button will remove an entry.  Below the \e Developers section is where
* the \e Testsuites that are to be run are defined.  The \e Testsuites are split
* to process each manifest individually so we enter the corresponding manifest
* name for the \e Testsuite in the same row. Once again you can click the \e
* Delete button to remove an entry.
*
* The \e Preview area at the bottom of the page displays a representation of the
* header that will be generated by the \e Implementation \e Report program.
*
* Below the \e Preview area are the links required to generate the file from the
* data in the form.  This is a two step process as the JSON data needs to be
* generated locally before it can be downloaded.  The generation is performed
* when clicking the \e Create \e JSON \e file button, once this step has been
* performed it is possible to download this by clicking the \e Click \e to \e
* download \e JSON link.
*
* \section usage_sec Usage
*
* To run the implementation report writer navigate to the build folder and run
* <pre>./test/implementation-report/implementation-report [json config file]
* [location of the test executables]</pre> this willl output the result to the
* console from where you can redirect to a file.  For example:
* <pre>./test/implementation-report/implementation-report jsonld-cpp.json
* test/testjsonld-cpp/ &gt; test_results.turtle</pre>
*
* \section tech_link Technical Details
*
* For a walkthrough please start at main()
*/

int main(int argc, char **argv)
{
/**
 * \section main_sec Technical Details
 * The program first reads the command line arguments and if there are
 * less than the required two then it returns the usage information to the
 * console and then terminates.
 *
 */
    if ( argc < 2 )
    {
        std::cerr << usage(argv[0]) << std::endl;
        return 1;
    }

/**
 * Assuming that was successful the next thing we need to do is verify
 * that the arguments that were provided are sane.  The first argument should
 * point to a file that is used to configure the report \ref config_sec and we
 * use it to initialise a ConfigReader()
 *
 */
    // parse the configuration file
    ConfigReader cr ( argv[1] );

/**
 * Now comes the time where we want to run our tests and get the data that is
 * going to populate our implementation report.  We do this with the aptly
 * named TestRunner() that is going to use our second argument passed to the
 * program, which is the location of the test executables to run.  It also
 * needs the names of the testsuites that we're going to run which are included
 * in the configuration file that we've just read.
 *
 * There might be a case for also including the location of the test files in
 * the configuration file too so that we only need to use one argument when
 * launching the program.  The problem with that is that we use a webpage at the
 * moment to create the configuration file and we would need to get the user to
 * type or copy and paste the path as a text string because we can't select
 * a folder/path using javascript for security reasons.  As I predominately use
 * the linux command line, it is easier for me to use bash completion to enter
 * a relative path.
 *
 */
    // run the testsuites
    TestRunner tr ( argv[2], cr.getTestsuites() );
    tr.start();

 /**
  * At this point we have all the data that we need but it's all in a bit of a
  * jumble so we need to get it all into a standard format, ready for it to be
  * output.  RDF is used as it's structure can be transformed into any of the
  * formats expected to be used for an implementation report.  Our
  * ReportBuilder() will be used to parse different types of data into RDF,
  * ready to be output in the final stage.
  *
  */
    // create RdfDataBuilder
    RdfDataBuilder db;

 /**
  * Lets start building our implementation report by getting the subjects defned
  * in the configuration file.  Each of these is in JSON format and will be
  * parsed to RDF and added to our dataset in a linear way -- i.e. the first
  * parsed element will be the first element in the data, the second will
  * follow that, and so on....
  *
  * Once all of these have been parsed we have our header for the implementation
  * report.
  */
    // parse the configuration header
    for ( const auto& j : cr.getPrefixes() )
    {
        db.parsePrefix( j );
    }
    for ( const auto& j : cr.getSubjects() )
    {
        db.parse( j );
    }

 /**
  * We get the important information here, the TestResult() that are provided
  * iteratively by the TestRunner::has_next() method and converted to RDF
  * linearly until there are no further TestResults to be returned.  It's worth
  * noting here that even if there are multiple test executables, the
  * TestRunner::next_result() method will keep seamlessly returning the next
  * one until results from all the test executables have been returned.
  *
  */
    // parse the results
    while ( tr.has_next() )
    {
        db.parse( tr.next_result() );
    }

 /**
  * That's all our data realised and converted to RDF data now so the next thing
  * we want to do is define somewhere to write our implementation report out
  * to.  The ReportWriter() is what we use to abstract away the output stream
  * that we are going to use.
  *
  */
    // Initialize the report writer
    ReportWriter rw = ReportWriter( std::cout );

 /**
  * We now have something to write the implementation report to but we need a
  * way to transform our data into the required format.  This is where the
  * EarlFormatter() comes in.
  *
  */
    // create the output formatter
    EarlFormatter ef;

 /**
  * Putting it all together, we ask the ReportWriter() to write the
  * RDFDataBuilder::database after passing to the EarlFormatter() and converting
  * it to the required output format.
  */
    // output the results
    rw.write ( ef.str( db.database ) );
    return 0;
}

