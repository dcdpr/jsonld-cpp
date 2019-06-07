#ifndef LIBJSONLD_CPP_JSONLDERROR_H
#define LIBJSONLD_CPP_JSONLDERROR_H

#include <utility>
#include "json.hpp"

class JsonLdError {
public:

    class Error {
    private:
        std::string error;

    public:
        Error()
                : error("")
        {}

        explicit Error(std::string error)
                : error(std::move(error))
        {}

        std::string toString() const
        {
            return error;
        }

    };

    const static Error LoadingDocumentFailed;
        const static Error ListOfLists;
        const static Error InvalidIndexValue;
        const static Error ConflictingIndexes;
        const static Error InvalidIdValue;
        const static Error InvalidLocalContext;
        const static Error MultipleContextLinkHeaders;
        const static Error LoadingRemoteContextFailed;
        const static Error InvalidRemoteContext;
        const static Error RecursiveContextInclusion;
        const static Error InvalidBaseIri;
        const static Error InvalidVocabMapping;
        const static Error InvalidDefaultLanguage;
        const static Error KeywordRedefinition;
        const static Error InvalidTermDefinition;
        const static Error InvalidReverseProperty;
        const static Error InvalidIriMapping;
        const static Error CyclicIriMapping;
        const static Error InvalidKeywordAlias;
        const static Error InvalidTypeMapping;
        const static Error InvalidLanguageMapping;
        const static Error CollidingKeywords;
        const static Error InvalidContainerMapping;
        const static Error InvalidTypeValue;
        const static Error InvalidValueObject;
        const static Error InvalidValueObjectValue;
        const static Error InvalidLanguageTaggedString;
        const static Error InvalidLanguageTaggedValue;
        const static Error InvalidTypedValue;
        const static Error InvalidSetOrListObject;
        const static Error InvalidLanguageMapValue;
        const static Error CompactionToListOfLists;
        const static Error InvalidReversePropertyMap;
        const static Error InvalidReverseValue;
        const static Error InvalidReversePropertyValue;
        const static Error SyntaxError;
        const static Error NotImplemented;
        const static Error UnknownFormat;
        const static Error InvalidInput;
        const static Error ParseError;
        const static Error UnknownError;

private:
    Error type;
    nlohmann::json detail;

public:
    JsonLdError(JsonLdError::Error type, nlohmann::json detail) {
        detail = (detail == nullptr ? "" : detail);
        this->type = std::move(type);
    }

    explicit JsonLdError(JsonLdError::Error type) {
        detail = "";
        this->type = std::move(type);
    }

    const Error &getType() const;

};


#endif //LIBJSONLD_CPP_JSONLDERROR_H
