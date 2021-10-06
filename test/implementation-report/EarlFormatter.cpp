#include <ctime>
#include "EarlFormatter.h"

EarlFormatter::EarlFormatter()
{
    // insert initial namespace
    RdfNamespace ns;
    ns.uri = "http://www.w3.org/ns/earl#";
    ns.prefix = "earl";
    namespaces.insert( ns );
}

std::string EarlFormatter::assertion(
        const std::string& subject,
        const std::string& user,
        const std::string& test,
        const std::string& result)
{
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));

    std::string outcome;
    ( result == "OK" ) ? outcome = "passed" : outcome = "failed";
    std::stringstream ss;
    ss  << "["
        << std:: endl << "  a <http://www.w3.org/ns/earl#Assertion>;"
        << std:: endl << "  <http://www.w3.org/ns/earl#assertedBy> <" << user << ">;"
        << std:: endl << "  <http://www.w3.org/ns/earl#mode> <http://www.w3.org/ns/earl#automatic>;" 
        << std:: endl << "  <http://www.w3.org/ns/earl#result> ["
        << std:: endl << "    a <http://www.w3.org/ns/earl#TestResult>;"
        << std:: endl << "    dc:date \"" << buf << "\"^^xsd:date;"
        << std:: endl << "    <http://www.w3.org/ns/earl#outcome> <http://www.w3.org/ns/earl#" << outcome << ">"
        << std:: endl << "  ];"
        << std:: endl << "  <http://www.w3.org/ns/earl#subject> <" << subject << ">;"
        << std:: endl << "  <http://www.w3.org/ns/earl#test> <" << test << ">"
        << std:: endl << "] .";
    return ss.str();
}

std::string EarlFormatter::format( RdfData* data )
{
   std::stringstream ss;
   // check if RdfData has a subject, if not then this is the subject and
   // needs to be encapsulated in square brackets
   if ( data->subject.name == "" )
   {
       ss << "[ ";
   } else {
       ss << data->subject.name;
       addNamespace( data->subject.ns );
   }
   // process children
   for ( auto o : data->objects )
   {
       format( o );
   };
   // finalize
   if ( data->objects.empty() )
   {
       ss << " ]";
   }
   // complete the triplet declaration
   ss << " .";

   return ss.str();
}

void EarlFormatter::appendRdfObject( std::stringstream& ss, RdfObject& obj )
{
    if ( obj.ns.prefix != "" ){
        ss << obj.ns.prefix << ".";
        // save the namespaces to be used in the prefix
        namespaces.insert( obj.ns );
    }
    ss << obj.name;
}

std::string EarlFormatter::prefix( )
{
    std::stringstream ss;
    for ( auto ns : namespaces )
    {
        if ( ns.uri != "" )
            ss << "@prefix " << ns.prefix << ": <" << ns.uri << "> ." << std::endl;
    }
    return ss.str();
}

void EarlFormatter::addRdfData( RdfData* rd )
{
    // parse the subject
    parseRdfData( rd );
    data.insert( rd );
}

void EarlFormatter::parseRdfData( RdfData* d )
{
    // process subject
    if ( d->subject.ns.uri != "" )
    {
        namespaces.insert( d->subject.ns );
    }
    // recursively process children
    for ( auto c : d->objects )
    {
        parseRdfData( c );
    }
}

void EarlFormatter::addNamespace( RdfNamespace& p )
{
    namespaces.insert( p );
}

std::string EarlFormatter::str()
{
    std::stringstream ss;
    // add prefix data
    ss << prefix();
    // add rdf data
    for ( auto d : data )
    {
        ss << format(d);
    }
    return ss.str();
}

