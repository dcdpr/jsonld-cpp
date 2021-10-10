#ifndef INPL_REPORT_RDFDATA
#define INPL_REPORT_RDFDATA

#include <ostream>
#include <string>
#include <vector>

class RdfNamespace
{
    public:

        // members
        std::string uri;
        std::string prefix;

        // constructors
        RdfNamespace(){};
        RdfNamespace( std::string uri, std::string prefix ): uri{uri}, prefix{prefix} {};

        ~RdfNamespace(){};

        // overlad the equality operator
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
        // overlad the stream output operator
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
        RdfObject(){};
        RdfObject( const std::string s ): name{s}{};
        RdfObject( RdfNamespace ns, std::string s ): ns{ns}, name{s}{};

        ~RdfObject(){};

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
        // overlad the stream output operator
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

        RdfData(){};
        RdfData( std::string s ) : subject{ RdfObject( s ) }{};
        RdfData( RdfObject t ) : subject{ t }{};
        RdfData( RdfObject t, RdfData* v )
        {
            subject = t;
            addChild( v );
        }

        ~RdfData(){
            // delete child data
            for ( auto rd : objects )
            {
                delete rd;
            }
        };
        
        void addChild( RdfData* );
        void addChild( RdfObject& );
        void addChild( RdfObject&, RdfData* );
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
        // overlad the stream output operator
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


#endif //INPL_REPORT_RDFDATA
