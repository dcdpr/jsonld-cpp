#ifndef LIBJSONLD_CPP_CONTEXT_H
#define LIBJSONLD_CPP_CONTEXT_H

#include <utility>
#include <memory>
#include "JsonLdConsts.h"
#include "JsonLdUtils.h"
#include "JsonLdOptions.h"
#include "json.hpp"
#include "JsonLdError.h"

class Context {
public:
    typedef std::map<std::string, std::string> StringMap;

private:

    JsonLdOptions options;
    nlohmann::json termDefinitions;
    nlohmann::json inverse;
    StringMap contextMap;

    void checkEmptyKey(nlohmann::json map);
    void checkEmptyKey(std::map<std::string, std::string> map);
    void createTermDefinition(json context, std::string term, std::map<std::string, bool> & defined);
    nlohmann::json getTermDefinition(std::string key);


    void init();


public:

    Context() {
    }

    explicit Context(JsonLdOptions options)
            : options(std::move(options))
    {
        init();
    }

    Context(std::map<std::string, std::string> map, JsonLdOptions options)
            : options(std::move(options)), contextMap(std::move(map)) {
        checkEmptyKey(contextMap); // todo: move this to init?
        init();
    }

    Context(std::map<std::string, std::string> map)
            : contextMap(std::move(map)) {
        checkEmptyKey(contextMap);
        init();
    }


    Context parse(json localContext, std::vector<std::string> remoteContexts, bool parsingARemoteContext);

    Context parse(json localContext, std::vector<std::string> remoteContexts);

    Context parse(json localContext);


    /**
 * Retrieve container mapping.
 *
 * @param property
 *            The Property to get a container mapping for.
 * @return The container mapping if any, else null
 */
    std::string getContainer(std::string property);


    std::string expandIri(std::string value, bool relative, bool vocab);
    std::string expandIri(std::string value, bool relative, bool vocab, json context, std::map<std::string, bool> & defined);
    json expandValue(std::string activeProperty, json value);




    bool isReverseProperty(const std::string& property);

    std::string & at(const std::string& s);
    size_t erase( const std::string& key );
    std::pair<StringMap::iterator,bool> insert( const StringMap::value_type& value );
    size_t count( const std::string& key ) const;
    void printInternalMap();

};


#endif //LIBJSONLD_CPP_CONTEXT_H
