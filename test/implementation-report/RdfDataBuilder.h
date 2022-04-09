#ifndef IMPL_REPORT_RDFDATABUILDER_H
#define IMPL_REPORT_RDFDATABUILDER_H

#include "RdfData.h"
#include "TestResult.h"
#include "jsoninc.h"

class RdfDataBuilder
{
public:
    virtual ~RdfDataBuilder();

    std::vector<RdfData*> database;
    void parse( const TestResult& );
    void parse( const nlohmann::json& );
    RdfObject parseObject( const std::string& );
    RdfObject parseSimpleObject( const std::string& );
    void parsePrefix(const nlohmann::json &p);
    RdfNamespace parseNamespace( const std::string& );
    RdfData* get( const std::string& );
    RdfData* search( RdfData*, std::string );
private:
    std::string get_time();

};

#endif //IMPL_REPORT_RDFDATABUILDER_H
