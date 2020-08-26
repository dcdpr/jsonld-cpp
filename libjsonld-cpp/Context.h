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
    nlohmann::json inverse;
    StringMap contextMap;

    static void checkEmptyKey(const nlohmann::json& map);
    static void checkEmptyKey(const StringMap& map);
    void createTermDefinition(nlohmann::json context, const std::string& term, std::map<std::string, bool> & defined);
    nlohmann::json getTermDefinition(const std::string & key);

    void init();

public:

    Context() = default;
    explicit Context(JsonLdOptions options);
    Context(std::map<std::string, std::string> map, JsonLdOptions options);
    explicit Context(std::map<std::string, std::string> map);

// todo: should these be static constructors?
    Context parse(const nlohmann::json & localContext, std::vector<std::string> & remoteContexts, bool parsingARemoteContext);
    Context parse(const nlohmann::json & localContext, std::vector<std::string> & remoteContexts);
    Context parse(const nlohmann::json & localContext);

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

    std::string & at(const std::string& s);
    size_t erase( const std::string& key );
    std::pair<StringMap::iterator,bool> insert( const StringMap::value_type& value );
    size_t count( const std::string& key ) const;
};

#endif //LIBJSONLD_CPP_CONTEXT_H
