#include <ctime>
#include "EarlFormatter.h"

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

std::string EarlFormatter::format( RdfData& data )
{
   std::stringstream ss;
   // check if RdfData has a subject, if not then this is the subject and
   // needs to be encapsulated in square brackets
   if ( data.triplet.subject == "" )
   {
       ss << "[ ";
   } else {
       ss << data.triplet.subject;
   }
   // complete the triplet declaration
   ss << data.triplet.subject << " ";
   appendRdfObject( ss, data.triplet.predictate );
   ss << " ";
   appendRdfObject( ss, data.triplet.object.at(0) ); // TODO handle multiple objects
   ss << ";" << std::endl; 
   // add the attributes
   for ( auto att : data.attributes )
   {
       appendRdfObject( ss, att.attribute );
       ss << " " << att.value << ";" << std::endl;
   }
   // close the encapsulation if there is no subject
   if (data.triplet.subject == "" ) ss << "]";
   // mark the end of the data with a .
   ss << " .";

   return ss.str();
}

void EarlFormatter::appendRdfObject( std::stringstream& ss, RdfObject& obj )
{
    if ( obj.ns.prefix != "" ) ss << obj.ns.prefix << ".";
    ss << obj.name;
}
