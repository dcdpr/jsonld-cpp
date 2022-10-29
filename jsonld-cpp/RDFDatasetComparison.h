#ifndef LIBJSONLD_CPP_RDFDATASETCOMPARISON_H
#define LIBJSONLD_CPP_RDFDATASETCOMPARISON_H

#include "jsonld-cpp/RDFDataset.h"

// Thanks to titanium-json-ld for this class

namespace RDF {

    bool areIsomorphic(const RDFDataset & dataset1, const RDFDataset & dataset2);

}

#endif //LIBJSONLD_CPP_RDFDATASETCOMPARISON_H
