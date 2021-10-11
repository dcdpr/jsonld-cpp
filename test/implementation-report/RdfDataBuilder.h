#ifndef IMPL_REPORT_RDFDATABUILDER_H
#define IMPL_REPORT_RDFDATABUILDER_H

#include "RdfData.h"
#include "TestResult.h"
#include "jsoninc.h"

class RdfDataBuilder
{
    public:
        void parse( const TestResult& );
        void parse( const nlohmann::json& );
        RdfObject parseObject( const std::string& );
        RdfNamespace parseNamespace( const std::string& );
        std::vector<RdfData*> database;
    private:
        std::string getTime();
};

#endif //IMPL_REPORT_RDFDATABUILDER_H
