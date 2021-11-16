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
        depth++;
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
    else
    {
        // close the array for empty name
        if ( obj.name == "" )
        {
            ss << "]" << std::endl;
            depth--;
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
    ss << std::endl;
    return ss.str();
}

void EarlFormatter::addNamespace( RdfNamespace& p )
{
    namespaces.insert( p );
}

std::string EarlFormatter::str( std::vector<RdfData*> data )
{
    std::stringstream ss;
    // add rdf data
    for ( auto d : data )
    {
        // process subject
        if ( d->subject.ns.uri != "" )
        {
            namespaces.insert( d->subject.ns );
        }
        // add Rdf to output
        format( ss, d );
        ss << '.' << std::endl;
    }
    // add prefix data
    return prefix() + ss.str();
}

