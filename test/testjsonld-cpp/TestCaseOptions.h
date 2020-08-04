#ifndef LIBJSONLD_CPP_TESTCASEOPTIONS_H
#define LIBJSONLD_CPP_TESTCASEOPTIONS_H

#include <JsonLdOptions.h>
#include "jsoninc.h"

struct TestCaseOptions {
    static TestCaseOptions create(nlohmann::json o, std::string baseUri);
    void copyTo(JsonLdOptions & jsonLdOptions);

    std::string specVersion;
    std::string base;
    std::string processingMode;
    bool normative = false;
    std::string expandContext;
    bool compactArrays = false;
    bool compactToRelative = false;
    std::string rdfDirection;
    bool produceGeneralizedRdf = false;
    bool useNativeTypes = false;
    bool useRdfType = false;
    bool omitGraph = false;

};


#endif //LIBJSONLD_CPP_TESTCASEOPTIONS_H
