#ifndef IMPL_REPORT_EARLFORMATTER
#define IMPL_REPORT_EARLFORMATTER

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include "RdfData.h"

/**
 * \brief Formatter to take RdfData and transform it to Earl
 */
class EarlFormatter
{
    public:
        //constructors
        EarlFormatter();

        //methods
        std::string prefix (  );
        void format( std::stringstream&, RdfData*, int = 0 );
        std::string str( std::vector<RdfData*> );

    private:
        // members
        std::set<RdfNamespace> namespaces;
        // methods
        void addNamespace( RdfNamespace& );
};

#endif //IMPL_REPORT_EARLFORMATTER
