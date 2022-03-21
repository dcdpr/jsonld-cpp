#ifndef IMPL_REPORT_RDFDATA
#define IMPL_REPORT_RDFDATA

#include <ostream>
#include <string>
#include <utility>
#include <vector>

class RdfNamespace
{
    public:

        // members
        std::string uri;
        std::string prefix;

        // constructors
        RdfNamespace() = default;
        RdfNamespace( std::string iuri, std::string iprefix )
                : uri{std::move(iuri)}, prefix{std::move(iprefix)} {}

        // overload the equality operator
        friend int operator== ( const RdfNamespace& lhs, const RdfNamespace& rhs)
        {
            // can't have two prefixes that are the same
            return lhs.prefix == rhs.prefix;
        }
        // overload the greater than operator
        friend int operator< ( const RdfNamespace& lhs, const RdfNamespace& rhs)
        {
            return lhs.prefix < rhs.prefix;
        }
        // overload the stream output operator
        friend std::ostream& operator<< ( std::ostream& os, const RdfNamespace& ns)
        {
            os << "NAMESPACE  "
                << "prefix: " << ns.prefix
                << " uri: " << ns.uri;
            return os;
        }

};


class RdfObject
{
    public:
        // members
        RdfNamespace ns;
        std::string name;

        // constructors
        RdfObject() = default;
        explicit RdfObject( std::string s )
                : name{std::move(s)}{}
        RdfObject( RdfNamespace ins, std::string s )
                : ns{std::move(ins)}, name{std::move(s)}{}


        // overload the equality operator
        friend int operator== ( const RdfObject& lhs, const RdfObject& rhs)
        {
            // can't have two prefixes that are the same
            return ( lhs.ns == rhs.ns &&
                    lhs.name == rhs.name );
        }
        // overload the greater than operator
        friend int operator< ( const RdfObject& lhs, const RdfObject& rhs)
        {
            return ( lhs.ns < rhs.ns &&
                    lhs.name == rhs.name );
        }
        // overload the stream output operator
        friend std::ostream& operator<< ( std::ostream& os, const RdfObject& o )
        {
            os << "OBJECT  "
                << "name: " << o.name
                << "  " << o.ns;
            return os;
        }
};


class RdfData
{
    public:
        RdfObject subject;
        std::vector<RdfData*> objects;

        RdfData() = default;
        explicit RdfData( std::string s ) : subject{ RdfObject( s ) }{}
        explicit RdfData( RdfObject t ) : subject{std::move( t )}{}
        RdfData( RdfObject t, RdfData* v )
        {
            subject = std::move(t);
            addChild( v );
        }

        ~RdfData(){
            // delete child data
            for ( auto rd : objects )
            {
                delete rd;
            }
        }

        void addChild( RdfData* );
        void addChild( RdfObject& );
        void addChild( RdfObject&, RdfData* );
        RdfObject getValue();

        // overload the equality operator
        friend int operator== ( const RdfData& lhs, const RdfData& rhs)
        {
            // can't have two prefixes that are the same
            return lhs.subject == rhs.subject;
        }
        // overload the greater than operator
        friend int operator< ( const RdfData& lhs, const RdfData& rhs)
        {
            return lhs.subject < rhs.subject;
        }
        // overload the stream output operator
        friend std::ostream& operator<< ( std::ostream& os, const RdfData& d )
        {
            os << "DATA  "
                << "subject: " << d.subject << std::endl;
            for ( auto o : d.objects )
            {
                os << "  " << *o << std::endl;
            }
            return os;
        }
};


#endif //IMPL_REPORT_RDFDATA
