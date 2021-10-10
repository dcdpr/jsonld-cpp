#include <iostream>
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

void EarlFormatter::format( std::stringstream& ss, RdfData* data, int depth )
{
    // check if RdfData has a subject, if not then this is the subject and
    // needs to be encapsulated in square brackets

    // get the main object
    auto& obj = data->subject;
    // is the object empty? If so begin an array
    if ( obj.name == "" )
    {
        ss << "[ ";
    }
    else
    {
        // check for a namespace
        if ( obj.ns.prefix != "" )
        {
            addNamespace( obj.ns );
            ss << obj.ns.prefix << ':' << obj.name;
        } else {
            // check for last element in data
            // TODO: better way of handling "a"
            if ( data->objects.empty() && obj.name != "a" )
            {
                ss << '"' << obj.name << '"';
            } else {
                ss << obj.name;
            }
        }
        ss << ' ';
    }
    // process children
    for ( auto o : data->objects )
    {
        format( ss, o, depth++ );
    };
    // finalize
    if ( data->objects.empty() )
    {
        if ( depth > 0 )
        {
            ss << ";" << std::endl;
        }
    }

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
        format( ss, d );
        ss << '.' << std::endl;
    }
    return ss.str();
}

