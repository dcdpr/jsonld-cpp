#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include <string>
#include <memory>

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/JsonLdOptions.h"


class Context {
private:

    nlohmann::ordered_json termDefinitions;

    JsonLdOptions options;

    std::string baseIRI;
    std::string originalBaseURL;

    std::string vocabularyMapping;
    std::string defaultLanguage;

    std::string defaultBaseDirection;
    std::shared_ptr<Context> previousContext;
    std::shared_ptr<Context> inverseContext;


public:

    Context() = default;
    explicit Context(const JsonLdOptions& options);


    nlohmann::ordered_json getTermDefinition(const std::string & key) const;

    bool isReverseProperty(const std::string& property) const;
    bool isProcessingMode(const std::string& mode) const;


    const std::string &getBaseIri() const;
    void setBaseIri(const std::string &baseIri);

    const std::string &getOriginalBaseUrl() const;
    void setOriginalBaseUrl(const std::string &originalBaseUrl);

    const std::string &getDefaultLanguage() const;
    void setDefaultLanguage(const std::string &defaultLanguage);

    const std::string &getVocabularyMapping() const;
    void setVocabularyMapping(const std::string &vocabularyMapping);

    const std::shared_ptr<Context> &getPreviousContext() const;

    void setPreviousContext(const std::shared_ptr<Context> &previousContext);

    const std::shared_ptr<Context> &getInverseContext() const;

    void setInverseContext(const std::shared_ptr<Context> &inverseContext);

    std::string getDefaultBaseDirection() const;

    const JsonLdOptions &getOptions() const;

    void setDefaultBaseDirection(const std::string & direction);

    const nlohmann::ordered_json &getTermDefinitions() const;

    nlohmann::ordered_json &getTermDefinitions();
};

#endif //LIBJSONLD_CPP_CONTEXT_H
