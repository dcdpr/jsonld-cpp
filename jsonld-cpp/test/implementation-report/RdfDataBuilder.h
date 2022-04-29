#ifndef IMPL_REPORT_RDFDATABUILDER_H
#define IMPL_REPORT_RDFDATABUILDER_H

#include "RdfData.h"
#include "TestResult.h"
#include <jsonld-cpp/jsoninc.h>
#include <regex>

/**
 * \brief   Builds and stores an RDF data structure
 *
 * \details We needed to have an internal format to store our data and so
 *          chose to create a basic implementation of the <a
 *          href="https://www.w3.org/TR/rdf11-concepts/" target="_blank">RDF
 *          Spec</a> to create a database.  The main reason for this was that
 *          the transformation to the turtle format, required for the
 *          implementation report, is a recognised transformation target for
 *          RDF.  It would also allow the flexability to support other output
 *          formats in the future if needed.
 *
 *          Input data can be parsed from several formats to add to the data
 *          stored: Header data is provided by the configuration tool as a JSON
 *          string; TestResult data is provided as a class instance.
 */
class RdfDataBuilder
{
public:
    virtual ~RdfDataBuilder();

    /**
     * \brief   Member where we store the RDF Data tree
     */
    std::vector<RdfData*> database;
    /**
     * \param   tr A TestResult() containing the results of a single test
     *
     * \brief   A method to add TestReult data to our stored RDF data
     *
     * \details The TestResult data
     */
    void parse( const TestResult& tr);
    /**
     * \param   json A JSON structure containing header data
     *
     * \brief   A method to add JSON header data to our stored RDF data
     *
     * \details The JSON Data is read from the configuration file and the
     *          "subject" section of the "header" needs to be stored.  This
     *          data contains the \e doap and \e foaf data in the format:
     * \code
     * {
     *     "id" : <identifier>,
     *     "type" : <object>,
     *     "value" : <value>,
     *     "properties" : [
     *         {
     *             "type" : <object>,
     *             "value" : <value>
     *         }
     *     ]
     * }
     * \endcode
     */
    void parse( const nlohmann::json& json);
    void parsePrefix(const nlohmann::json &p);
private:
    /**
     * \brief   Regular Expression used to identify a string representation
     *          of a URL
     */
    static std::regex const url_regex;
    /**
     * \brief   Helper method to get the current date and time as a string
     *
     * \return  the current date and time as a string in the format
     *          2011-10-08T07:07:09Z
     */
    std::string get_time();
    /**
     * \param   str Object definition.  This could be a URL, keyword or
     *          null value.
     *
     * \details Our RDF data will contain an object which could be defined
     *          by a URL or a
     */
    RdfObject parseObject( const std::string& str);
    RdfObject parseSimpleObject( const std::string& str);
    /**
     * \param   str URI that needs to be translated into a RdfNamepace
     *
     * \return  An instance of a RdfNamespace()
     *
     * \details A URI string can be substituted throughout the RDF data by a
     *          prefix.  This method takes a URI and extracts a string from the
     *          URI to use as the prefix
     */
    RdfNamespace parseNamespace( const std::string& str);
    RdfData* get( const std::string& str);
    RdfData* search( RdfData*, const std::string& str );

    RdfData *parseProperty(const nlohmann::json &json);
};

#endif //IMPL_REPORT_RDFDATABUILDER_H
