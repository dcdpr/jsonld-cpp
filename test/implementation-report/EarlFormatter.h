#ifndef IMPL_REPORT_EARLFORMATTER
#define IMPL_REPORT_EARLFORMATTER

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include "RdfData.h"

/**
 * \brief A formatter to convert RDF data to the Earl
 *
 * \details RDF data can be transformed to a number of different formats, one of
 *          them is the <a
 *          href="https://www.w3.org/WAI/standards-guidelines/earl/"
 *          target="_blank">Evaluatioon and Report Language \e (EaRL)</a>.
 *          This class has been created to handle the transformation of
 *          RdfData() to \e EaRL.
 *
 *          The processing of the RDF data is not strictly done as a stream as
 *          in the RDF data we have RdfNamespace() objects that are collected
 *          and then used to create a \e prefix section at the head of the Earl
 *          output.
 *
 * \code
 *          @prefix dc: <http://purl.org/dc/terms/> .
 *          @prefix dcdpr: <https://github.com/dcdpr/> .
 *          @prefix doap: <http://usefulinc.com/ns/doap#> .
 *          @prefix earl: <http://www.w3.org/ns/earl#> .
 *          @prefix foaf: <http://xmlns.com/foaf/0.1/> .
 *          @prefix github: <https://github.com/> .
 *
 *          <>  foaf:primaryTopic dcdpr:jsonld-cpp ;
 *              foaf:maker github:dpape ;
 *          .
 *          dcdpr:jsonld-cpp a doap:project ;
 *              doap:name "jsonld-cpp" ;
 *              doap:description "A C++ library implementing the JSONLD API" ;
 *              doap:homepage dcdpr:jsonld-cpp ;
 *              doap:developer github:hawkmauk ;
 *          .
 *          github:hawkmauk a foaf:Person ;
 *              foaf:name "Michael Hawkins" ;
 *          .
 *
 * \endcode
 */
class EarlFormatter
{
    public:
        /**
         * \breif   A simple constructor for this class
         *
         * \details We need an instance of the EarlFormatter as as it's going to 
         *          collect prefix data from the RDF data that is parses so we
         *          can't just use class methods.
         */
        EarlFormatter();

        /**
         * \brief The destructor for this class
         */
        ~EarlFormatter(){};

        /**
         * \brief Returns a formatted string containing prefix data
         */
        std::string prefix (  );
        
        /**
        * \param    std::vector<RdfData*> data The RDF Dataset that is to be
        *           formatted
        *
        * \return   std::string A representation of the RDF dataset as a string
        *           in the Earl format
        *
        * \details  The choice to have a str method was to keep the interface
        *           simple and abstract away a lot of the complexity of
        *           formatting the RDF data.  This has resulted in a fairly
        *           strict but ultimately simpler way to transform the RDF
        *           data.
        */
        std::string str( std::vector<RdfData*> );

    private:
        /**
         * \details This is used to store namespaces that have been extraced
         *          from the RDF Data, to be used in generating a prefix.  We 
         *          don't expose this member as it is specifically used for this
         *          task only.
         */
        std::set<RdfNamespace> namespaces;
        /**
         * \brief   Method to add \RdfNamespace to the namespaces member
         * \details We've used a set as the collection which we save the
         *          namespaces to but might want to change this at a later date
         *          so we abstract the implementation details away by adding
         *          this method.  It's private so that might not be strictly
         *          neccessary but hey.
         */
        void addNamespace( RdfNamespace& );
        /**
         * \brief   Helper method used by EarlFormatter::str() to create the
         *          Earl output
         *
         * \param   std::stringstream ss Which is going to be used to output our
         *          EaRL formatted data
         * \param   RdfData* data The RDF data that is to be transformed by the
         *          formatter and output to the std::stringstream
         * \param   int depth Is used when this method is called recursively to
         *          determine how deep the nesting of the data is.
         *
         * \details This method is the workhorse of the class and traverses the
         *          RDF data tree to convert it into Earl and output it to the
         *          std::stringstream that has been provided.
         */
        void format( std::stringstream&, RdfData*, int = 0 );
};

#endif //IMPL_REPORT_EARLFORMATTER
