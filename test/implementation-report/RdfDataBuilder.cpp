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

void RdfDataBuilder::parse( const TestResult& tr )
{
    // start of an array like object with now subject
    // subject is this so should be blank
    RdfData* data = new RdfData( );
    // define the namespace
    RdfNamespace ns( "http://www.w3.org/ns/earl#", "earl");

    // define the predicate
    RdfObject p("a");
    RdfData* predictate = new RdfData( p );
    // define the object
    RdfObject o( ns, "Assertion" );
    predictate->addChild( new RdfData( o ) );
    // add to the data object
    data->addChild( predictate );
   
    //set the subject
//    RdfObject s( ns, "subject" );
//    RdfData* subject = new RdfData( s );
    // find the project
//    RdfData* project = get( "project" );
//    RdfObject project_obj = project->getValue();
//    subject->addChild( project_obj );
    // add to the data object
//    data->addChild( subject );

    // set the assertedBy
    RdfObject a( ns, "assertedBy" );
    RdfData* assertedBy = new RdfData( a ); 
    // find the maker
    RdfData* maker = get( "maker" );
    RdfObject maker_obj = maker->getValue();
    assertedBy->addChild( maker_obj );
    // add to the data object
    data->addChild( assertedBy );


    RdfObject test( ns, "test" );
    data->addChild(
            test,
            new RdfData( tr.manifest + "#" + tr.test )
            );
    RdfObject mode( ns, "mode" );
    data->addChild(
            mode,
            new RdfData( "automatic" )
            );
    RdfObject result( ns, "result" );
    data->addChild(
            result,
            new RdfData( tr.result )
            );

    this->database.push_back(data);
}

void RdfDataBuilder::parse( const nlohmann::json& json )
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
    this->database.push_back( d );
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

RdfData* RdfDataBuilder::get( std::string s )
{
    // recursively search database for search string
    
    for ( std::vector<RdfData*>::iterator it = this->database.begin(); it != this->database.end() ; ++it )
    {
        auto i= *it;
        auto d = search( i, s );
        if ( d != nullptr )
            return d;
    }
    return nullptr;
}

RdfData* RdfDataBuilder::search( RdfData* data, std::string s )
{
    for ( auto d : data->objects )
    {
        if ( d->subject.name == s )
        {
            return d;
        }
        else
        {
            // recursively search
            auto r = search( d, s );
            if ( r != nullptr ) return r;
        }
    }
    return nullptr;
}
