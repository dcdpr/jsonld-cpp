#include <iostream>
#include <ctime>
#include "EarlFormatter.h"

EarlFormatter::EarlFormatter()
{
    /**
     * We initialise the EarlFormatter by first creating a RdfNamespace() that
     * contains the \e earl prefix and uri of the definition.
     */
    RdfNamespace ns;
    ns.uri = "http://www.w3.org/ns/earl#";
    ns.prefix = "earl";
    /**
     * Once the namespace has been created, we use the private method
     * EarlFormatter::addNamespace() to add it to our collection.
     */
    addNamespace( ns );
}

void EarlFormatter::format( std::stringstream& ss, RdfData* data, int depth )
{
    // check if RdfData has a subject, if not then this is the subject and
    // needs to be encapsulated in square brackets

    // check for nullptr and do nothing if found
    if ( data == nullptr ) return;

    // get the main object
    auto& obj = data->subject;
    // is the object empty? If so begin an array
    if ( obj.name.empty() )
    {
        ss << "[ ";
        depth++;
    }
    else
    {
        // check for a namespace
        if ( !obj.ns.prefix.empty() )
        {
            addNamespace( obj.ns );
            ss << obj.ns.prefix << ':' << obj.name;
        } else {
            // check for last element in data
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
    }

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
        if ( obj.name.empty() )
        {
            ss << "] ";// << std::endl;
            depth--;
        }
    }
}

std::string EarlFormatter::prefix( )
{
    std::stringstream ss;
    for ( const auto& ns : namespaces )
    {
        if ( !ns.uri.empty() )
            ss << "@prefix " << ns.prefix << ": <" << ns.uri << "> ." << std::endl;
    }
    ss << std::endl;
    return ss.str();
}

void EarlFormatter::addNamespace( RdfNamespace& p )
{
    namespaces.insert( p );
}

std::string EarlFormatter::str( const std::vector<RdfData*>& data )
{
    /**
     * As we are going to be building up our output we use a std::stringstream
     * object to sequentially write data to.
     */
    std::stringstream ss;
    /**
     * We iterate over the vector that has been provided
     */
    for ( auto d : data )
    {
        /**
         * skipping any iteration that contains a nullptr.
         */
        if ( d == nullptr ) continue;

        /**
         * If the RdfData() contains a RdfNamespace() then we store it so that
         * it can be used to generate a prefix.
         */
        if ( !d->subject.ns.uri.empty() )
        {
            addNamespace( d->subject.ns );
        }
        /**
         * We then call the format() function, passing the RdfData() and the
         * std::stringstream
         */
        format( ss, d );
        /**
         * After each set of RdfData() we terminate the output by adding a
         * full stop character and extra newline for formatting.
         */
        ss << ".\n" << std::endl;
    }
    /**
     * Once all of the vector elements have been iterated through we are ready
     * to return the Earl formatted string by concatinating the prefix() output
     * with the std::stringstream.
     */
    return prefix() + ss.str();
}

