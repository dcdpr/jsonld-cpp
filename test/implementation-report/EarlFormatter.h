#ifndef IMPL_REPORT_EARLFORMATTER
#define IMPL_REPORT_EARLFORMATTER

#include <string>
#include <sstream>
#include "RdfData.h"

class EarlFormatter
{
    public:
        std::string assertion(
                const std::string& subject,
                const std::string& user,
                const std::string& test,
                const std::string& result);

        std::string format( RdfData& );

    private:
        void appendRdfObject( std::stringstream&, RdfObject& );
};

#endif //IMPL_REPORT_EARLFORMATTER
