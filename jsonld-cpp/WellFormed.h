#ifndef LIBJSONLD_CPP_WELLFORMED_H
#define LIBJSONLD_CPP_WELLFORMED_H


#include <string>

class WellFormed {

public:
    WellFormed() = delete;

    /**
     * Is 'str' a well-formed IRI or not?
     *
     * @param str string to examine
     * @return true is str is a well-formed IRI
     */
    static bool iri(const std::string& str);

    /**
     * Is 'str' a well-formed blank node identifier or not?
     *
     * @param str string to examine
     * @return true is str is a well-formed blank node identifier
     */
    static bool blankNodeIdentifier(const std::string& str);

    /**
     * Is 'str' a well-formed language tag or not?
     *
     * A language tag is well-formed according to section
     * 2.2.9 of [BCP47].
     *
     * @param str string to examine
     * @return true is str is a well-formed language tag
     */
    static bool language(const std::string& str);

    /**
     * Is 'str' a well-formed literal or not?
     *
     * A literal is well-formed if it has the lexical form of a string, any
     * datatype IRI is well-formed, and any language tag is well-formed
     * according to section 2.2.9 of [BCP47].
     *
     * @param str string to examine
     * @return true is str is a well-formed literal
     */
    static bool literal(const std::string& str);

    /**
     * Is 'str' a well-formed RDF 'subject' or not?
     *
     * An RDF 'subject' can be an IRI or a blank node.
     *
     * See: https://www.w3.org/TR/rdf11-concepts/#dfn-rdf-triple
     *
     * @param str string to examine
     * @return true is str is a well-formed RDF 'subject'
     */
    static bool rdf_subject(const std::string& str);


    /**
     * Is 'str' a well-formed RDF 'predicate' or not?
     *
     * An RDF 'predicate' can be an IRI.
     *
     * See: https://www.w3.org/TR/rdf11-concepts/#dfn-rdf-triple
     *
     * @param str string to examine
     * @return true is str is a well-formed RDF 'predicate'
     */
    static bool rdf_predicate(const std::string& str);


    /**
     * Is 'str' a well-formed RDF 'object' or not?
     *
     * An RDF 'object' can be an IRI, a literal, or a blank node.
     *
     * See: https://www.w3.org/TR/rdf11-concepts/#dfn-rdf-triple
     *
     * @param str string to examine
     * @return true is str is a well-formed RDF 'object'
     */
    static bool rdf_object(const std::string& str);

    /**
     * Is 'str' a well-formed RDF 'graph name' or not?
     *
     * An RDF 'graph name' can be an IRI or a blank node.
     *
     * See: https://www.w3.org/TR/rdf11-concepts/#dfn-graph-name
     *
     * @param str string to examine
     * @return true is str is a well-formed RDF 'object'
     */
    static bool rdf_graph_name(const std::string& str);

};


#endif //LIBJSONLD_CPP_WELLFORMED_H
