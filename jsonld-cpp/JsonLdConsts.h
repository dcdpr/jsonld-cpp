#ifndef LIBJSONLD_CPP_JSONLDCONSTS_H
#define LIBJSONLD_CPP_JSONLDCONSTS_H

/**
 * Constants used in the JSON-LD parser.
 */

namespace JsonLdConsts {

    static constexpr const char RDF_SYNTAX_NS[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
    static constexpr const char RDF_SCHEMA_NS[] = "http://www.w3.org/2000/01/rdf-schema#";
    static constexpr const char XSD_NS[] = "http://www.w3.org/2001/XMLSchema#";

    static constexpr const char XSD_ANYTYPE[] = "http://www.w3.org/2001/XMLSchema#anyType";
    static constexpr const char XSD_BOOLEAN[] = "http://www.w3.org/2001/XMLSchema#boolean";
    static constexpr const char XSD_DOUBLE[] = "http://www.w3.org/2001/XMLSchema#double";
    static constexpr const char XSD_INTEGER[] = "http://www.w3.org/2001/XMLSchema#integer";
    static constexpr const char XSD_FLOAT[] = "http://www.w3.org/2001/XMLSchema#float";
    static constexpr const char XSD_DECIMAL[] = "http://www.w3.org/2001/XMLSchema#decimal";
    static constexpr const char XSD_ANYURI[] = "http://www.w3.org/2001/XMLSchema#anyURI";
    static constexpr const char XSD_STRING[] = "http://www.w3.org/2001/XMLSchema#string";

    static constexpr const char RDF_TYPE[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
    static constexpr const char RDF_FIRST[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#first";
    static constexpr const char RDF_REST[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#rest";
    static constexpr const char RDF_NIL[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#nil";
    static constexpr const char RDF_PLAIN_LITERAL[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#PlainLiteral";
    static constexpr const char RDF_XML_LITERAL[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#XMLLiteral";
    static constexpr const char RDF_OBJECT[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#object";
    static constexpr const char RDF_LANGSTRING[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#langString";
    static constexpr const char RDF_LIST[] = "http://www.w3.org/1999/02/22-rdf-syntax-ns#List";

    static constexpr const char TEXT_TURTLE[] = "text/turtle";
    static constexpr const char APPLICATION_NQUADS[] = "application/n-quads";

    static constexpr const char FLATTENED[] = "flattened";
    static constexpr const char COMPACTED[] = "compacted";
    static constexpr const char EXPANDED[] = "expanded";

    static constexpr const char ID[] = "@id";
    static constexpr const char DEFAULT[] = "@default";
    static constexpr const char GRAPH[] = "@graph";
    static constexpr const char CONTEXT[] = "@context";
    static constexpr const char PRESERVE[] = "@preserve";
    static constexpr const char EXPLICIT[] = "@explicit";
    static constexpr const char OMIT_DEFAULT[] = "@omitDefault";
    static constexpr const char EMBED_CHILDREN[] = "@embedChildren";
    static constexpr const char EMBED[] = "@embed";
    static constexpr const char LIST[] = "@list";
    static constexpr const char LANGUAGE[] = "@language";
    static constexpr const char INDEX[] = "@index";
    static constexpr const char SET[] = "@set";
    static constexpr const char TYPE[] = "@type";
    static constexpr const char REVERSE[] = "@reverse";
    static constexpr const char VALUE[] = "@value";
    static constexpr const char ATNULL[] = "@null";
    static constexpr const char NONE[] = "@none";
    static constexpr const char ANY[] = "@any";
    static constexpr const char CONTAINER[] = "@container";
    static constexpr const char BLANK_NODE_PREFIX[] = "_:";
    static constexpr const char VOCAB[] = "@vocab";
    static constexpr const char BASE[] = "@base";
    static constexpr const char REQUIRE_ALL[] = "@requireAll";

    // v1.1
    static constexpr const char VERSION[] = "@version";
    static constexpr const char DIRECTION[] = "@direction";
    static constexpr const char IMPORT[] = "@import";
    static constexpr const char PROPAGATE[] = "@propagate";
    static constexpr const char PROTECTED[] = "@protected";
    static constexpr const char JSON[] = "@json";
    static constexpr const char NEST[] = "@nest";
    static constexpr const char PREFIX[] = "@prefix";
    static constexpr const char INCLUDED[] = "@included";

    static constexpr const char JSON_LD_1_0[] = "json-ld-1.0";
    static constexpr const char JSON_LD_1_1[] = "json-ld-1.1";
    static constexpr bool DEFAULT_COMPACT_ARRAYS = true;

    // various flags used for term definitions
    static constexpr const char IS_PREFIX_FLAG[] = "@flag.prefix";
    static constexpr const char IS_PROTECTED_FLAG[] = "@flag.protected";
    static constexpr const char IS_REVERSE_PROPERTY_FLAG[] = "@flag.reverseproperty";

    // used to store other json values in the term definitions
    static constexpr const char LOCALCONTEXT[] = "@td.localcontext";
    static constexpr const char BASEURL[] = "@td.baseurl";

    enum Embed {
        ALWAYS, NEVER, ONCE, LINK
    };

}

#endif //LIBJSONLD_CPP_JSONLDCONSTS_H
