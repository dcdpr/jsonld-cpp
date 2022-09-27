#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/JsonLdError.h"
#include <utility>
#include <memory>

class Context {
public:
    typedef std::map<std::string, std::string> StringMap;

private:

    JsonLdOptions options;
    nlohmann::json termDefinitions;
    std::string baseIRI;
    std::string originalBaseURL;

    Context *inverseContext{};
    std::string vocabularyMapping;
    std::string defaultLanguage;
    std::string defaultBaseDirection; // todo: this should be an enum
    std::shared_ptr<Context> previousContext;

    StringMap contextMap; // todo: should probably replace this with several specific variables


    std::vector<std::string> remoteContexts;
    bool overrideProtected{};
    bool propagate{};
    bool validateScopedContext{};


    void createTermDefinition(
            nlohmann::json localContext,
            const std::string& term,
            std::map<std::string, bool> & defined,
            std::string baseURL = "",
            bool isProtected = false,
            bool overrideProtected = false,
            std::vector<std::string> remoteContexts = std::vector<std::string>(),
            bool validateScopedContext = true
            );

    std::string & at(const std::string& s);
    size_t erase( const std::string& key );
    std::pair<StringMap::iterator,bool> insert( const StringMap::value_type& value );
    size_t count( const std::string& key ) const;

public:

    Context() = default;
    explicit Context(const JsonLdOptions& options);

    Context parse(const nlohmann::json & localContext, const std::string & baseURL);
    Context parse(const nlohmann::json & localContext, const std::string & baseURL,
                  std::vector<std::string> & remoteContexts,
                  bool overrideProtected = false,
                  bool propagate = true,
                  bool validateScopedContext = true);

    nlohmann::json getTermDefinition(const std::string & key);

    std::string expandIri(std::string value, bool relative, bool vocab);
    std::string expandIri(std::string value, bool relative, bool vocab, const nlohmann::json& context, std::map<std::string, bool> & defined);
    nlohmann::json expandValue(const std::string & activeProperty, const nlohmann::json& value);
    bool isReverseProperty(const std::string& property);
    bool isProcessingMode(const std::string& mode);

    void setDefaultBaseDirection(const std::string & direction);
    std::string getDefaultBaseDirection() const;

    Context *getPreviousContext() const;

    const std::string &getBaseIri() const;

    void setBaseIri(const std::string &baseIri);

    const std::string &getOriginalBaseUrl() const;

    void setOriginalBaseUrl(const std::string &originalBaseUrl);

};

#endif //LIBJSONLD_CPP_CONTEXT_H
