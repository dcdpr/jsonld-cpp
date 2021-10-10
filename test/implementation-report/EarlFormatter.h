#ifndef IMPL_REPORT_EARLFORMATTER
#define IMPL_REPORT_EARLFORMATTER

#include <string>
#include <sstream>
#include <set>
#include "RdfData.h"

class EarlFormatter
{
    public:
        //constructors
        EarlFormatter();
        std::string assertion(
                const std::string& subject,
                const std::string& user,
                const std::string& test,
                const std::string& result);

        //methods
        std::string prefix (  );
        void format( std::stringstream&, RdfData*, int = 0 );
        void addRdfData( RdfData* );
        std::string str();

    private:
        // members
        std::set<RdfNamespace> namespaces;
        std::set<RdfData*> data;
        // methods
        void appendRdfObject( std::stringstream&, RdfObject& );
        void addNamespace( RdfNamespace& );
        void parseRdfData( RdfData* );
};

#endif //IMPL_REPORT_EARLFORMATTER
