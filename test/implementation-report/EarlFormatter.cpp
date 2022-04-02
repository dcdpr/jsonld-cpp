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
    // check for nullptr and do nothing if found
    if ( data == nullptr ) return;

    /**
     * The journey through transforming a RDF dataset to EaRL starts here but
     * to begin with lets recap on how the RDF data is composed.  We have two
     * elements: A subject which is an RdfObject() which itself is comprised of
     * a RdfNamespace() and a string, then it can optionally also have 0 or more
     * RdfData() objects.
     *
     * \code
     * RdfData
     * - subject (RdfObject)
     *   - ns (RdfNamespace)
     *      - prefix (string)
     *      - uri (string)
     *   - name (string)
     * - objects (vector<RdfData*>)
     * \endcode
     *
     * to begin we check if the current RDF data node has a subject, if not
     * then this is an array of Rdf data nodes and needs to be encapsulated in
     * square brackets.  To keep a track of our depth, we increment a variable
     * that is 0 when we are at the top of the structure.
     */

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
        /**
         * If we do have a subject then we check for a namespace and add it to
         * the namespaces member so that it can be included in the prefix.  We
         * then append the prefix and name to the output, delimited with a
         * colon.
        */
        {
            addNamespace( obj.ns );
            ss << obj.ns.prefix << ':' << obj.name;
        }
        else
        /**
         * Otherwise we need to check if this is the last element in the data,
         * in which case we encapsulate the name in double quotes.  There is a
         * condition where the object name is the keyword 'a' which doesn't have
         * a namespace, in this instance we simply output the keyword 'a'.
        */
        {
            // check for last element in data
            if ( data->objects.empty() && obj.name != "a" )
            {
                ss << '"' << obj.name << '"';
            } else {
                ss << obj.name;
            }
        }
        // add a trailing space
        ss << ' ';
    }

    /**
     * For each of the RddData pointers, we recursively call the format function,
     * incrementing the depth as we do.
     */
    for ( auto o : data->objects )
    {
        format( ss, o, depth++ );
    };

    /** 
     *  After we have finished traversing the child, it's time to termniate
     *  the elements that we've created.  If we are at the end of the RdfData
     *  and the top of the tree then we close off with a semi-colon.  Otherwise
     *  we check to see if the subject has a name, if not then we are at the
     *  end of the array and can terminate it with a right bracket and decrease
     *  the depth.
     */
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
    /**
     * As we are going to be building up our output we use a std::stringstream
     * object to sequencially write data to.
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
        if ( d->subject.ns.uri != "" )
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
         * full stop character.
         */
        ss << '.' << std::endl;
    }
    /**
     * Once all of the vector elements have been iterated through we are ready
     * to return the Earl formatted string by concatinating the prefix() output
     * with the std::stringstream.
     */
    return prefix() + ss.str();
}

