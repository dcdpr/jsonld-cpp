#ifndef LIBJSONLD_CPP_IRIUTILS_H
#define LIBJSONLD_CPP_IRIUTILS_H

#include <string>

namespace IriUtils {
    /**
     * Removes dot segments from a url path.
     *
     * @param path the path to remove dot segments from
     * @param hasAuthority true if the url has an authority, false if not
     * @return the URL without the dot segments
     */
    std::string removeDotSegments(const std::string& path, bool hasAuthority);

    /**
     * Prepends a base IRI to the given relative IRI
     *
     * @param base the base IRI
     * @param iri the relative IRI
     * @return the absolute IRI
     */
    std::string prependBase(std::string base, std::string iri);

    /**
     * Returns if an IRI is absolute
     *
     * An IRI is absolute if, and only if, it has a scheme component.
     *
     * @param iri the IRI to test
     * @return true if the IRI is absolute
     */
    bool isIriAbsolute(const std::string& iri);
}

#endif //LIBJSONLD_CPP_IRIUTILS_H
