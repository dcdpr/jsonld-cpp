#ifndef IMPL_REPORT_RDFDATA
#define IMPL_REPORT_RDFDATA

#include <ostream>
#include <string>
#include <utility>
#include <vector>

/**
 * \brief   Provides context with a URI to a definition and a prefix that can
 *          be used as a reference to the URI
 *
 * \details A RDF Namespace contains a URI and a prefix that can be used as a
 *          substitute for the URI, making the data more human readable.
 *          The URI can either be used inline in the data or replaced inline
 *          with the prefix and then included in a context section.
 */
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

        /**
         * \brief   When comparing two RdfNamespace(), we look to the prefix and
         *          use the value to determine equality.
         *
         * \details It is not possible to have multiple namespaces with the same
         *          prefix so we overload the equality operator to compare the
         *          value of prefix in two RdfNamespace() instances.
         */
        friend int operator== ( const RdfNamespace& lhs, const RdfNamespace& rhs)
        {
            // can't have two prefixes that are the same
            return lhs.prefix == rhs.prefix;
        }
        /**
         * \details When comparing two RdfNamespace(), we look to the prefix and
         *          use the value to determine order.
         */
        friend int operator< ( const RdfNamespace& lhs, const RdfNamespace& rhs)
        {
            return lhs.prefix < rhs.prefix;
        }
        /**
         * \brief    We define the way that a RdfNamespace() should be displayed
         *          when passed to an output stream
         *
         * \details This is in a format suitable for debugging and does not
         *          conform to any recognised specification.
         */
        friend std::ostream& operator<< ( std::ostream& os, const RdfNamespace& ns)
        {
            os << "NAMESPACE  "
                << "prefix: " << ns.prefix
                << " uri: " << ns.uri;
            return os;
        }

};

/**
 * \brief   A description of an entity in an RDF graph.  This could be a subject
 *          predicate or object.
 *
 * \detail  This class implemets a representation of RDF Nodes and Arcs as
 *          described in the <a
 *          href="https://www.w3.org/TR/rdf-primer/#rdfmodel" target="_blank">
 *          RDF Primer</a>.  Each is a constituant of a RDF Triple that is
 *          built using RdfData() objects.
 *
 *          As a full RDF implementation was not a desired goal for this
 *          project, I chose to create a lite implementation that would provide
 *          the functionality needed to generate a report.
 */
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

        /**
         * \brief   When comparing two RdfObject(), we look to the namespace and
         *          name to determine equality.
         */
        friend int operator== ( const RdfObject& lhs, const RdfObject& rhs)
        {
            return ( lhs.ns == rhs.ns &&
                    lhs.name == rhs.name );
        }
        /**
         * \details When comparing two RdfObject(), we look to the namespace and
         *          name to determine order.
         */
        friend int operator< ( const RdfObject& lhs, const RdfObject& rhs)
        {
            return ( lhs.ns < rhs.ns &&
                    lhs.name == rhs.name );
        }
        /**
         * \brief   We define the way that a RdfObject() should be displayed
         *          when passed to an output stream
         *
         * \details This is in a format suitable for debugging and does not
         *          conform to any recognised specification.
         */
        friend std::ostream& operator<< ( std::ostream& os, const RdfObject& o )
        {
            os << "OBJECT  "
                << "name: " << o.name
                << "  " << o.ns;
            return os;
        }
};


/**
 * \brief   Used to build a RDF Tripple of RdfObjects() in an RDF graph.
 *
 * \detail  This class implemets a way to build RDF Tripples as defined in
 *          href="https://www.w3.org/TR/rdf-primer/#rdfmodel" target="_blank">
 *          RDF Primer</a> and <a
 *          href="https://www.w3.org/TR/2004/REC-rdf-concepts-20040210/#section-Graph-syntax"
 *          target="_blank">RDF Concepts</a>.  The purpose is to providee a way
 *          to create a linked tree of RDFData() instances that will create a
 *          RDF Tripple. A RdfData() instance is a relationship between a
 *          RdfObject() and other RdfData() in the RDF Tripple.
 *
 *          A RDF Tripple will consist of at least 2 RdfData() instances:
 *          \li Subject ->  Predicate relationship
 *          \li Predicate -> Object relationship
 *
 *          The subject can have multiple \e properties (Predicate -> Object
 *          relationships) so we implament a collection of these to
 *          build the linked tree structure.
 *
 *          As a full RDF implementation was not a desired goal for this
 *          project, I chose to create a lite implementation that would provide
 *          the functionality needed to generate a report.
 */
class RdfData
{
    public:
        /**
         * \brief   The subject of the RDF Tripple
         *
         * \detail  If this is the \e subject in the tripple it will have a
         *          value, otherwise this will be null.
         */
        RdfObject subject;
        /**
         * \brief   Collection storing the properties of the subject or object
         *          related to the predicte
         *
         * \detail  If this is the \e object in the tripple then there will be
         *          nothing in the collection
         */
        std::vector<RdfData*> objects;

        // constructors
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
