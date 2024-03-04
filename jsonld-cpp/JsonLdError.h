#ifndef LIBJSONLD_CPP_JSONLDERROR_H
#define LIBJSONLD_CPP_JSONLDERROR_H

#include "jsonld-cpp/jsoninc.h"

class JsonLdError : public std::runtime_error {
public:

    static const char LoadingDocumentFailed[];
    static const char ListOfLists[];
    static const char InvalidIndexValue[];
    static const char ConflictingIndexes[];
    static const char InvalidIdValue[];
    static const char InvalidLocalContext[];
    static const char MultipleContextLinkHeaders[];
    static const char LoadingRemoteContextFailed[];
    static const char InvalidRemoteContext[];
    static const char RecursiveContextInclusion[];
    static const char InvalidBaseIri[];
    static const char InvalidVocabMapping[];
    static const char InvalidDefaultLanguage[];
    static const char KeywordRedefinition[];
    static const char InvalidTermDefinition[];
    static const char InvalidReverseProperty[];
    static const char InvalidIriMapping[];
    static const char CyclicIriMapping[];
    static const char InvalidKeywordAlias[];
    static const char InvalidTypeMapping[];
    static const char InvalidLanguageMapping[];
    static const char CollidingKeywords[];
    static const char InvalidContainerMapping[];
    static const char InvalidTypeValue[];
    static const char InvalidValueObject[];
    static const char InvalidValueObjectValue[];
    static const char InvalidLanguageTaggedString[];
    static const char InvalidLanguageTaggedValue[];
    static const char InvalidTypedValue[];
    static const char InvalidSetOrListObject[];
    static const char InvalidLanguageMapValue[];
    static const char CompactionToListOfLists[];
    static const char InvalidReversePropertyMap[];
    static const char InvalidReverseValue[];
    static const char InvalidReversePropertyValue[];
    static const char SyntaxError[];
    static const char IllegalArgument[];

    static const char InvalidVersionValue[];
    static const char ProcessingModeConflict[];
    static const char InvalidProtectedValue[];
    static const char InvalidScopedContext[];
    static const char InvalidBaseDirection[];
    static const char InvalidNestValue[];
    static const char InvalidPrefixValue[];
    static const char InvalidContextNullification[];
    static const char ContextOverflow[];
    static const char InvalidContextEntry[];
    static const char InvalidImportValue[];
    static const char InvalidPropagateValue[];
    static const char InvalidIncludedValue[];
    static const char ProtectedTermRedefinition[];

    JsonLdError(const std::string& type, const nlohmann::ordered_json& detail);
    explicit JsonLdError(const std::string& type);

    const char* what() const noexcept override;

};


#endif //LIBJSONLD_CPP_JSONLDERROR_H
