#ifndef IMPL_REPORT_RDFDATABUILDER_H
#define IMPL_REPORT_RDFDATABUILDER_H

#include "RdfData.h"
#include "TestResult.h"
#include "jsoninc.h"

class RdfDataBuilder
{
    public:
        RdfData* parse( const TestResult& );
        RdfData* parse( const nlohmann::json& );
        RdfObject parseObject( const std::string& );
        RdfNamespace parseNamespace( const std::string& );
    private:
        std::string getTime();
};

#endif //IMPL_REPORT_RDFDATABUILDER_H
