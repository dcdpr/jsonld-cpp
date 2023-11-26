#include <jsonld-cpp/JsonLdUrl.h>
#include "TestCaseOptions.h"

namespace {

    std::string getEntryOr(const nlohmann::json &manifestEntry, const std::string & entryKey, const std::string & defaultEntry) {
        return manifestEntry.contains(entryKey) ?
               manifestEntry[entryKey].get<std::string>() : defaultEntry;
    }

    bool getBool(const nlohmann::json &manifestEntry, const std::string & entryKey) {
        return manifestEntry.contains(entryKey) && manifestEntry[entryKey].get<bool>();
    }

}

TestCaseOptions TestCaseOptions::create(nlohmann::json manifestEntryOptions, const std::string& baseUri) {
    TestCaseOptions result;

    result.specVersion = getEntryOr(manifestEntryOptions, "specVersion", "");

    result.base = getEntryOr(manifestEntryOptions, "base", "");

    result.processingMode = getEntryOr(manifestEntryOptions, "processingMode", "");

    result.normative = getBool(manifestEntryOptions, "normative");

    if(manifestEntryOptions.contains("expandContext")) {
        std::string context = manifestEntryOptions["expandContext"].get<std::string>();
        result.expandContext = JsonLdUrl::resolve(&baseUri, &context);
    }

    result.compactArrays = getBool(manifestEntryOptions, "compactArrays");

    result.compactToRelative = getBool(manifestEntryOptions, "compactToRelative");

    result.rdfDirection = getEntryOr(manifestEntryOptions, "rdfDirection", "");

    result.produceGeneralizedRdf = getBool(manifestEntryOptions, "produceGeneralizedRdf");

    result.useNativeTypes = getBool(manifestEntryOptions, "useNativeTypes");

    result.useRdfType = getBool(manifestEntryOptions, "useRdfType");

    result.omitGraph = getBool(manifestEntryOptions, "omitGraph");

    result.hashAlgorithm = getEntryOr(manifestEntryOptions, "hashAlgorithm", "");

    return result;
}

void TestCaseOptions::copyTo(JsonLdOptions & jsonLdOptions) {
    jsonLdOptions.setProcessingMode(processingMode);
    jsonLdOptions.setBase(base);
    if (!expandContext.empty()) {
        jsonLdOptions.setExpandContext(expandContext);
    }
    jsonLdOptions.setCompactArrays(compactArrays);
    jsonLdOptions.setCompactToRelative(compactToRelative);
    jsonLdOptions.setRdfDirection(rdfDirection);
    jsonLdOptions.setProduceGeneralizedRdf(produceGeneralizedRdf);
    jsonLdOptions.setUseNativeTypes(useNativeTypes);
    jsonLdOptions.setUseRdfType(useRdfType);
    jsonLdOptions.setOmitGraph(omitGraph);
}
