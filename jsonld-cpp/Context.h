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
    nlohmann::json termDefinitions; // used by context processor
    void setDefaultBaseDirection(const std::string & direction); // used by context processor

private:

    JsonLdOptions options;

    std::string baseIRI;
    std::string originalBaseURL;

    std::string vocabularyMapping;
    std::string defaultLanguage;

    std::string defaultBaseDirection; // todo: this should be an enum
    std::shared_ptr<Context> previousContext;
    std::shared_ptr<Context> inverseContext;


public:

    Context() = default;
    explicit Context(const JsonLdOptions& options);// used by context processor and used by jsonld processor


    nlohmann::json getTermDefinition(const std::string & key);// used by expansion processor

    bool isReverseProperty(const std::string& property) const; // reach into termdefinitions, used by expansion processor
    bool isProcessingMode(const std::string& mode) const; // reach into options, used by context and used by expansion processor


    const std::string &getBaseIri() const; // used by context processor
    void setBaseIri(const std::string &baseIri);// used by jsonld processor

    const std::string &getOriginalBaseUrl() const;// used by jsonld processor
    void setOriginalBaseUrl(const std::string &originalBaseUrl);// used by jsonld processor

    const std::string &getDefaultLanguage() const; // used by expansion processor
    void setDefaultLanguage(const std::string &defaultLanguage); // used by context processor

    const std::string &getVocabularyMapping() const; // used by context processor
    void setVocabularyMapping(const std::string &vocabularyMapping); // used by context processor

    const std::shared_ptr<Context> &getPreviousContext() const; // used by expansion processor

    void setPreviousContext(const std::shared_ptr<Context> &previousContext); // used by context processor

    const std::shared_ptr<Context> &getInverseContext() const;

    void setInverseContext(const std::shared_ptr<Context> &inverseContext);  // used by context processor

    std::string getDefaultBaseDirection() const; // used by expansion processor

    const JsonLdOptions &getOptions() const;// used by expansion processor

};

#endif //LIBJSONLD_CPP_CONTEXT_H
