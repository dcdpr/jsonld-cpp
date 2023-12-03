#include "jsonld-cpp/Context.h"
#include "jsonld-cpp/JsonLdUrl.h"
#include "jsonld-cpp/RemoteDocument.h"
#include <memory>

using nlohmann::json;


bool Context::isReverseProperty(const std::string &property) const {
    // todo: should move this function and others like it to a new TermDefinition class?
    if(!termDefinitions.contains(property)) {
        return false;
    }
    auto td = termDefinitions.at(property);
    if (td.is_null()) {
        return false;
    }
    return td.contains(JsonLdConsts::REVERSE) && td.at(JsonLdConsts::REVERSE);
}

nlohmann::json Context::getTermDefinition(const std::string & key) {
    if(termDefinitions.contains(key)) {
        return termDefinitions.at(key);
    }
    else
        return json::object();
}


Context::Context(const JsonLdOptions& ioptions)
        : options(ioptions)
{
    setBaseIri(options.getBase());
    termDefinitions = json::object();
    inverseContext = nullptr;
    previousContext = nullptr;
    defaultBaseDirection = "null";
}

bool Context::isProcessingMode(const std::string &mode) const{
    return options.getProcessingMode() == mode;
}

const std::string &Context::getBaseIri() const {
    return baseIRI;
}

void Context::setBaseIri(const std::string &baseIri) {
    baseIRI = baseIri;
}

const std::string &Context::getOriginalBaseUrl() const {
    return originalBaseURL;
}

void Context::setOriginalBaseUrl(const std::string &originalBaseUrl) {
    originalBaseURL = originalBaseUrl;
}

std::string Context::getDefaultBaseDirection() const {
    return defaultBaseDirection;
}

void Context::setDefaultBaseDirection(const std::string & direction) {
    defaultBaseDirection = direction;
}

const JsonLdOptions &Context::getOptions() const {
    return options;
}

const std::string &Context::getDefaultLanguage() const {
    return defaultLanguage;
}

void Context::setDefaultLanguage(const std::string &idefaultLanguage) {
    defaultLanguage = idefaultLanguage;
}

const std::string &Context::getVocabularyMapping() const {
    return vocabularyMapping;
}

void Context::setVocabularyMapping(const std::string &ivocabularyMapping) {
    vocabularyMapping = ivocabularyMapping;
}

const std::shared_ptr<Context> &Context::getPreviousContext() const {
    return previousContext;
}

void Context::setPreviousContext(const std::shared_ptr<Context> &previousContext) {
    Context::previousContext = previousContext;
}

const std::shared_ptr<Context> &Context::getInverseContext() const {
    return inverseContext;
}

void Context::setInverseContext(const std::shared_ptr<Context> &inverseContext) {
    Context::inverseContext = inverseContext;
}
