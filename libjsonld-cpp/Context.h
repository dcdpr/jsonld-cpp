#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include "jsoninc.h"
#include "JsonLdConsts.h"
#include "JsonLdUtils.h"
#include "JsonLdOptions.h"
#include "JsonLdError.h"
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
public:
    const std::string &getBaseIri() const;

    void setBaseIri(const std::string &baseIri);

    const std::string &getOriginalBaseUrl() const;

    void setOriginalBaseUrl(const std::string &originalBaseUrl);

private:
    Context *inverseContext;
    std::string vocabularyMapping;
    std::string defaultLanguage;
    std::string defaultBaseDirection;
    Context *previousContext;

    StringMap contextMap; // todo: should probably replace this with several specific variables


    std::vector<std::string> remoteContexts;
    bool overrideProtected;
    bool propagate;
    bool validateScopedContext;


    static void checkEmptyKey(const nlohmann::json& map);
    static void checkEmptyKey(const StringMap& map);
    void createTermDefinition(nlohmann::json context, const std::string& term, std::map<std::string, bool> & defined);
    nlohmann::json getTermDefinition(const std::string & key);

    void init();

    std::string & at(const std::string& s);
    size_t erase( const std::string& key );
    std::pair<StringMap::iterator,bool> insert( const StringMap::value_type& value );
    size_t count( const std::string& key ) const;

public:

    Context() = default;
    explicit Context(JsonLdOptions options);
//    Context(std::map<std::string, std::string> map, JsonLdOptions options);
//    explicit Context(std::map<std::string, std::string> map);

    Context parse(const nlohmann::json & localContext, const std::string & baseURL);
    Context parse(const nlohmann::json & localContext, const std::string & baseURL,
                  std::vector<std::string> & remoteContexts,
                  bool overrideProtected = false,
                  bool propagate = true,
                  bool validateScopedContext = true);

    /**
     * Retrieve container mapping.
     *
     * @param property
     *            The Property to get a container mapping for.
     * @return The container mapping if any, else null
     */
    std::string getContainer(std::string property);

    std::string expandIri(std::string value, bool relative, bool vocab);
    std::string expandIri(std::string value, bool relative, bool vocab, const nlohmann::json& context, std::map<std::string, bool> & defined);
    nlohmann::json expandValue(const std::string & activeProperty, const nlohmann::json& value);
    bool isReverseProperty(const std::string& property);
    bool isProcessingMode(const std::string& mode);

};

#endif //LIBJSONLD_CPP_CONTEXT_H
