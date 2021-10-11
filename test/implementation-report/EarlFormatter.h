#ifndef IMPL_REPORT_EARLFORMATTER
#define IMPL_REPORT_EARLFORMATTER

#include <string>
#include <sstream>
#include <vector>
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
        std::string str( std::vector<RdfData*> );

    private:
        // members
        std::set<RdfNamespace> namespaces;
        // methods
        void appendRdfObject( std::stringstream&, RdfObject& );
        void addNamespace( RdfNamespace& );
        void parseRdfData( RdfData* );
};

#endif //IMPL_REPORT_EARLFORMATTER
