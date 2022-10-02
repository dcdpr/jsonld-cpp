#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <utility>


class Context {
public:
    typedef std::map<std::string, std::string> StringMap;

private:

    JsonLdOptions options;

    std::string baseIRI;
    std::string originalBaseURL;

    Context *inverseContext{};
    std::string vocabularyMapping;
    std::string defaultLanguage;

    std::string defaultBaseDirection; // todo: this should be an enum
    std::shared_ptr<Context> previousContext;

    std::vector<std::string> remoteContexts;
    bool overrideProtected{};
    bool validateScopedContext{};

    void setDefaultBaseDirection(const std::string & direction);


public:
    const std::string &getDefaultLanguage() const;

    void setDefaultLanguage(const std::string &defaultLanguage);

    const std::string &getVocabularyMapping() const;

    void setVocabularyMapping(const std::string &vocabularyMapping);

    nlohmann::json termDefinitions;
    bool propagate{};

    Context() = default;
    explicit Context(const JsonLdOptions& options);// used by context and used by jsonld processor

    Context process(const nlohmann::json & localContext, const std::string & baseURL);// used by jsonld processor and expansion processor
    Context process(const nlohmann::json & localContext, const std::string & baseURL,// used by context and used by expansion processor
                  std::vector<std::string> & remoteContexts,
                    bool overrideProtected = false,
                    bool propagate = true,
                    bool validateScopedContext = true);

    nlohmann::json getTermDefinition(const std::string & key);// used by expansion processor

    std::string expandIri(std::string value, bool relative, bool vocab); // used by context and used by expansion processor


    bool isReverseProperty(const std::string& property); // reach into termdefinitions, used by expansion processor
    bool isProcessingMode(const std::string& mode); // reach into options, used by context and used by expansion processor


    std::string getDefaultBaseDirection() const; // used by expansion processor

    Context *getPreviousContext() const;// used by expansion processor

    const std::string &getBaseIri() const;

    void setBaseIri(const std::string &baseIri);// used by jsonld processor

    const std::string &getOriginalBaseUrl() const;// used by jsonld processor

    void setOriginalBaseUrl(const std::string &originalBaseUrl);// used by jsonld processor

    /**
     * Returns mutable reference to the JsonLdOptions object used by this context so that
     * other algorithms that use this context can modify option settings as needed.
     */
    JsonLdOptions &getOptions();// used by expansion processor

};

#endif //LIBJSONLD_CPP_CONTEXT_H
