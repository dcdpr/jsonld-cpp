#include <deque>
#include <regex>
#include <iostream>

#include "RdfDataBuilder.h"

std::regex const RdfDataBuilder::url_regex{ "^\\D+:\\/\\/.*" };

std::string RdfDataBuilder::get_time()
{
    time_t now;
    time(&now);
    char buf[sizeof "2011-10-08T07:07:09Z"];
    strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
    return buf;
}

void RdfDataBuilder::parse( const TestResult& tr )
{
    if(tr.empty())
        return;

    // start of an array like object with now subject
    // subject is this so should be blank
    RdfData* data = new RdfData( );
    // define the namespace
    RdfNamespace ns( "http://www.w3.org/ns/earl#", "earl");
    RdfNamespace dc( "http://purl.org/dc/terms/", "dc");

    // define the predicate
    RdfObject p("a");
    RdfData* predicate = new RdfData(p );
    // define the object
    RdfObject o( ns, "Assertion" );
    predicate->addChild(new RdfData(o ) );
    // add to the data object
    data->addChild(predicate );

    // set the assertedBy
    RdfObject ab( ns, "assertedBy" );
    RdfData* assertedBy = new RdfData( ab );
    // find the org
    RdfData* org = get( "foaf:Organization" );
    assertedBy->addChild( new RdfData( org->subject.name ) );
    // add to the data object
    data->addChild( assertedBy );

    //set the subject
    RdfObject s( ns, "subject" );
    RdfData* subject = new RdfData( s );
    // find the project
    RdfData* project = get( "doap:Project" );
    subject->addChild( new RdfData( project->subject.name ) );
    // add to the data object
    data->addChild( subject );

    RdfObject test( ns, "test" );
    data->addChild(
            test,
            new RdfData( tr.manifest + "#" + tr.test )
            );
    RdfObject mode( ns, "mode" );
    RdfObject mode_value;
    mode_value.ns = ns;
    mode_value.name = "automatic";
    data->addChild(
            mode,
            new RdfData( mode_value )
            );

    // create the result sub data
    RdfData* result_data = new RdfData( "earl:result" );
    RdfData* result_predicate = new RdfData( p );
    RdfObject result_object( ns, "TestResult" );
    result_predicate->addChild( new RdfData( result_object ) );
    result_data->addChild( result_predicate );

    RdfObject result_key( ns, "outcome" );
    RdfObject result_value;
    result_value.ns = ns;
    ( tr.result == "OK" ) ? result_value.name = "passed" : result_value.name = "failed";
    result_data->addChild(
            result_key,
            new RdfData( result_value )
            );
    // create datetime for when this test was run. Note that this isn't really the time the test
    // ran, but the time this implementation report is running and creating this report output.
    RdfObject date( dc, "date" );
    result_data->addChild(
            date,
            new RdfData( get_time() )
            );
    data->addChild( result_data );
    this->database.push_back(data);
}

void RdfDataBuilder::parse( const nlohmann::json& json )
{
    RdfData* d = new RdfData();
    // convert the id
    d->subject = parseSimpleObject( json["id"] );
    // convert the type
    auto type = parseSimpleObject( json["type"] );
    d->addChild( type );
    // convert the value
    auto value = parseSimpleObject( json["value"] );
    d->addChild( value );
    // process properties
    for ( auto p : json["properties"] )
    {
        // check to see if there is an id
        if(p.contains("id"))
            d->addChild(parseProperty(p));
        else {
            auto t = parseSimpleObject(p["type"]);
            auto v = parseSimpleObject(p["value"]);
            d->addChild(t, new RdfData(v));
        }
    }
    this->database.push_back( d );
}

RdfData* RdfDataBuilder::parseProperty( const nlohmann::json& json )
{
    RdfData* d = new RdfData();
    // convert the id
    d->subject = parseSimpleObject( json["id"] );
    // convert the type
    auto type = parseSimpleObject( json["type"] );
    //d->addChild( type );
    // convert the value
    auto value = parseSimpleObject( json["value"] );
    d->addChild( type, new RdfData(value) );
    // process properties
    for ( auto p : json["properties"] )
    {
        auto t = parseSimpleObject(p["type"]);
        auto v = parseSimpleObject(p["value"]);
        d->addChild(t, new RdfData(v));
    }
    return d ;
}

RdfObject RdfDataBuilder::parseObject( const std::string& str )
{

    /**
     * The first thing to do when parsing the string is to identify if this is
     * a URL that can shortened and added t the prefix section
     */
    if ( std::regex_match( str, RdfDataBuilder::url_regex ) )
    {
        std::cout << "Found URL: " << str << std::endl;
    }

    // get the last delimited value of the string
    // split the string and add to a vector
    // so that we can find the last element as the name
    std::stringstream ss(str);
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
            words.push_back( d2 );
        }
    }

    RdfObject o;
    // get the namespace if applicable
    // regexp match always returns the base string followed by
    // any matches
    if ( words.size() > 1 )
    {
        o.ns = parseNamespace( str );
    }

    if ( !words.empty() )
    {
        o.name = words.back();
    } else {
        o.name = "";
    }
    // set the name
    return o;
}

RdfObject RdfDataBuilder::parseSimpleObject( const std::string& str )
{
    RdfObject o;
    o.name = str;
    return o;
}

RdfNamespace RdfDataBuilder::parseNamespace( const std::string& str )
{
    //get the uri of the namespace
    std::smatch uri;
    std::regex_search(str, uri, std::regex("^.*[//#]" ) );

    // split the string and add to a set in reverse order
    // so that we can find the first sensible word to use
    // as the prefix
    std::stringstream ss(str);
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

void RdfDataBuilder::parsePrefix( const nlohmann::json& p )
{
    if(p.is_object() && p.size() == 1) {
        auto entry = p.begin();
        RdfNamespace ns( entry.value(), entry.key() );
        RdfObject o;
        o.ns = ns;
        o.name = "skipprefix"; // set name as special value so we can skip it later when printing in the EarlFormatter
        this->database.push_back(new RdfData(o));
    }
}

RdfData* RdfDataBuilder::get( const std::string& s )
{
    // recursively search database for search string
    for (auto i : this->database)
    {
        auto d = search( i, s );
        if ( d != nullptr )
            return i;
    }
    return nullptr;
}

RdfData* RdfDataBuilder::search( RdfData* data, const std::string& str )
{
    // check for null pointer
    if ( data == nullptr ) return data;

    for ( auto d : data->objects )
    {
        if ( d->subject.name == str )
        {
            return d;
        }
        else
        {
            // recursively search
            auto r = search( d, str );
            if ( r != nullptr ) return r;
        }
    }
    return nullptr;
}

RdfDataBuilder::~RdfDataBuilder() {
    for (auto i : this->database) {
        delete i;
    }
}
