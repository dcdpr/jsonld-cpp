#include <JsonLdUrl.h>
#include "TestCaseOptions.h"

TestCaseOptions TestCaseOptions::create(nlohmann::json o, std::string baseUri) {
    TestCaseOptions result;

    result.specVersion = o.contains("specVersion") ?
        o["specVersion"].get<std::string>() :
        "";

    result.base = o.contains("base") ?
        o["base"].get<std::string>() :
        "";

    result.processingMode = o.contains("processingMode") ?
        o["processingMode"].get<std::string>() :
        "";

    result.normative = o.contains("normative") && o["normative"].get<bool>();

    if(o.contains("expandContext")) {
        std::string context = o["expandContext"].get<std::string>();
        result.expandContext = JsonLdUrl::resolve(&baseUri, &context);
    }

    result.compactArrays = o.contains("compactArrays") && o["compactArrays"].get<bool>();

    result.compactToRelative = o.contains("compactToRelative") && o["compactToRelative"].get<bool>();

    result.rdfDirection = o.contains("rdfDirection") ?
        o["rdfDirection"].get<std::string>() :
        "";

    result.produceGeneralizedRdf = o.contains("produceGeneralizedRdf") && o["produceGeneralizedRdf"].get<bool>();

    result.useNativeTypes = o.contains("useNativeTypes") && o["useNativeTypes"].get<bool>();

    result.useRdfType = o.contains("useRdfType") && o["useRdfType"].get<bool>();

    result.omitGraph = o.contains("omitGraph") && o["omitGraph"].get<bool>();

    return result;
}

void TestCaseOptions::copyTo(JsonLdOptions & jsonLdOptions) {
    if (!processingMode.empty()) {
        jsonLdOptions.setProcessingMode(processingMode);
    }

    if (!base.empty()) {
        jsonLdOptions.setBase(base);
    }

    if (!expandContext.empty()) {
        jsonLdOptions.setExpandContext(expandContext);
    }

    if (compactArrays) {
        jsonLdOptions.setCompactArrays(compactArrays);
    }

//    if (compactToRelative) {
//        jsonLdOptions.setCompactToRelative(compactToRelative);
//    }

//    if (rdfDirection) {
//        jsonLdOptions.setRdfDirection(RdfDirection.valueOf(rdfDirection.toUpperCase().replace("-", "_")));
//    }

    if (produceGeneralizedRdf) {
        jsonLdOptions.setProduceGeneralizedRdf(produceGeneralizedRdf);
    }

    if (useNativeTypes) {
        jsonLdOptions.setUseNativeTypes(useNativeTypes);
    }

    if (useRdfType) {
        jsonLdOptions.setUseRdfType(useRdfType);
    }

    if (omitGraph) {
        jsonLdOptions.setOmitGraph(omitGraph);
    }

}
