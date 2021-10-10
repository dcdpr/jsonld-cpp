#include <deque>
#include <regex>
#include <iostream>

#include "RdfDataBuilder.h"

std::string RdfDataBuilder::getTime()
{
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    return buf;
}

RdfData* RdfDataBuilder::parse( const TestResult& tr )
{
    RdfNamespace ns( "http://www.w3.org/ns/earl#", "earl");
    //subject is this so should be blank
    //odefine the predicate
    RdfObject p("a");
    // define the object
    RdfObject o( ns, "Assertion" );
    // define the attributes
    RdfData* assertion = new RdfData( RdfObject ( "a" ) );

    RdfObject subject( ns, "subject" );
    assertion->addChild(
            subject,
            new RdfData( "project" )
    );
    RdfObject test( ns, "test" );
    assertion->addChild(
            test,
            new RdfData( tr.manifest + "#" + tr.test )
    );
    RdfObject assertedBy( ns, "assertedBy" );
    assertion->addChild(
            assertedBy,
            new RdfData( "maker" )
    );
    RdfObject mode( ns, "mode" );
    assertion->addChild(
            mode,
            new RdfData( "automatic" )
    );
    RdfObject result( ns, "result" );
    assertion->addChild(
            result,
            new RdfData( "project" )
    );

    return assertion;

}

RdfData* RdfDataBuilder::parse( const nlohmann::json& json )
{
    RdfData* d = new RdfData();
    // convert the id
    d->subject = parseObject( json["id"] );
    // convert the type
    auto type = parseObject( json["type"] );
    d->addChild( type );
    // convert the value
    auto value = parseObject( json["value"] );
    d->addChild( value );
    // process properties
    for ( auto p : json["properties"] )
    {
        auto t = parseObject( p["type"] );
        auto v = parseObject( p["value"] );
        d->addChild( t, new RdfData( v ) );
    }
    return d;
}

RdfObject RdfDataBuilder::parseObject( const std::string& s )
{
    //get the last delimited value of the string
    // split the string and add to a vector
    // so that we can find the last element as the name
    std::stringstream ss(s);
    std::string d1;
    std::vector<std::string> words;
    //initially split on folder separator
    while ( getline( ss, d1, '/' ) )
    {
        std::stringstream ss2(d1);
        std::string d2;
        //delimit on hash
        while ( getline( ss2, d2, '#' ) )
        {
            std::stringstream ss3(d2);
            std::string d3;
            // delimit on fullstop
            while( getline( ss3, d3, '.' ) )
            {
                words.push_back( d3 );
            }
        }
    }

    RdfObject o;
    // get the namespace if applicable
    // regexp match always returns the base string followed by
    // any matches
    if ( words.size() > 1 )
    {
        o.ns = parseNamespace( s );
    }
    
    if ( words.size() > 0 )
    {
        o.name = words.back();
    } else {
        o.name = "";
    }
    // set the name
    return o;
}

RdfNamespace RdfDataBuilder::parseNamespace( const std::string& s )
{
    //get the uri of the namespace
    std::smatch uri;
    std::regex_search( s, uri, std::regex( "^.*[//#]" ) );
    
    // split the string and add to a set in reverse order
    // so that we can find the first sensible word to use
    // as the prefix
    std::stringstream ss(s);
    std::string d1;
    std::deque<std::string> words;
    //initially split on folder separator
    while ( getline( ss, d1, '/' ) )
    {
        std::stringstream ss2(d1);
        std::string d2;
        //delimit on hash
        while ( getline( ss2, d2, '#' ) )
        {
            std::stringstream ss3(d2);
            std::string d3;
            // delimit on fullstop
            while( getline( ss3, d3, '.' ) )
            {
                words.push_front( d3 );
            }
        }
    }
    //search backwards for a suitable word
    std::smatch prefix;
    int match( 0 );
    for ( auto w : words )
    {
        //get the prefix of the namespace
        std::regex_search( w, prefix, std::regex( "[A-Za-z]{4,}" ) );
        if ( !prefix.empty() )
        {
            if ( match == 0 )
            {
                match++;
            } else {
                break;
            }
        }
    }
    
    RdfNamespace ns( uri[0], prefix[0] );
    return ns;
}
