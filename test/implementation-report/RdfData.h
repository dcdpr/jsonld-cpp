#ifndef INPL_REPORT_RDFDATA
#define INPL_REPORT_RDFDATA

#include <string>
#include <vector>
#include <set>

struct RdfNamespace
{
    std::string uri;
    std::string prefix;
};

struct RdfObject
{
    RdfNamespace ns;
    std::string name;
};

struct RdfTriplet
{
    std::string subject;
    RdfObject predictate;
    std::vector<RdfObject> object;
};

struct RdfAttribute
{
    RdfObject attribute;
    std::string value;
};

struct RdfData
{
    RdfTriplet triplet;
    std::set<RdfAttribute> attributes;
};

#endif //INPL_REPORT_RDFDATA
