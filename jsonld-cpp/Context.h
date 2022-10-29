#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include <string>
#include <memory>


class Context {
public:
    // todo: need to encapsulate these?
    Context *inverseContext{};
    std::shared_ptr<Context> previousContext;
    nlohmann::json termDefinitions;
    void setDefaultBaseDirection(const std::string & direction);

private:

    JsonLdOptions options;

    std::string baseIRI;
    std::string originalBaseURL;

    std::string vocabularyMapping;
    std::string defaultLanguage;

    std::string defaultBaseDirection; // todo: this should be an enum


public:

    Context() = default;
    explicit Context(const JsonLdOptions& options);// used by context processor and used by jsonld processor


    nlohmann::json getTermDefinition(const std::string & key);// used by expansion processor

    bool isReverseProperty(const std::string& property) const; // reach into termdefinitions, used by expansion processor
    bool isProcessingMode(const std::string& mode) const; // reach into options, used by context and used by expansion processor


    const std::string &getBaseIri() const;
    void setBaseIri(const std::string &baseIri);// used by jsonld processor

    const std::string &getOriginalBaseUrl() const;// used by jsonld processor
    void setOriginalBaseUrl(const std::string &originalBaseUrl);// used by jsonld processor

    const std::string &getDefaultLanguage() const;
    void setDefaultLanguage(const std::string &defaultLanguage);

    const std::string &getVocabularyMapping() const;
    void setVocabularyMapping(const std::string &vocabularyMapping);

    std::string getDefaultBaseDirection() const; // used by expansion processor

    Context *getPreviousContext() const;// used by expansion processor

    const JsonLdOptions &getOptions() const;// used by expansion processor

};

#endif //LIBJSONLD_CPP_CONTEXT_H
