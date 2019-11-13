#include "JsonLdError.h"

JsonLdError::JsonLdError(const std::string& itype, const nlohmann::json& idetail)
        : std::runtime_error(itype + idetail.get<std::string>())
{
}

JsonLdError::JsonLdError(const std::string& itype)
        : std::runtime_error(itype)
{
}

const char *JsonLdError::what() const noexcept {
    return runtime_error::what();
}

const char JsonLdError::LoadingDocumentFailed[] = "loading document failed";
const char JsonLdError::ListOfLists[] = "list of lists";
const char JsonLdError::InvalidIndexValue[] = "invalid @index value";
const char JsonLdError::ConflictingIndexes[] = "conflicting indexes";
const char JsonLdError::InvalidIdValue[] = "invalid @id value";
const char JsonLdError::InvalidLocalContext[] = "invalid local context";
const char JsonLdError::MultipleContextLinkHeaders[] = "multiple context link headers";
const char JsonLdError::LoadingRemoteContextFailed[] = "loading remote context failed";
const char JsonLdError::InvalidRemoteContext[] = "invalid remote context";
const char JsonLdError::RecursiveContextInclusion[] = "recursive context inclusion";
const char JsonLdError::InvalidBaseIri[] = "invalid base IRI";
const char JsonLdError::InvalidVocabMapping[] = "invalid vocab mapping";
const char JsonLdError::InvalidDefaultLanguage[] = "invalid default language";
const char JsonLdError::KeywordRedefinition[] = "keyword redefinition";
const char JsonLdError::InvalidTermDefinition[] = "invalid term definition";
const char JsonLdError::InvalidReverseProperty[] = "invalid reverse property";
const char JsonLdError::InvalidIriMapping[] = "invalid IRI mapping";
const char JsonLdError::CyclicIriMapping[] = "cyclic IRI mapping";
const char JsonLdError::InvalidKeywordAlias[] = "invalid keyword alias";
const char JsonLdError::InvalidTypeMapping[] = "invalid type mapping";
const char JsonLdError::InvalidLanguageMapping[] = "invalid language mapping";
const char JsonLdError::CollidingKeywords[] = "colliding keywords";
const char JsonLdError::InvalidContainerMapping[] = "invalid container mapping";
const char JsonLdError::InvalidTypeValue[] = "invalid type value";
const char JsonLdError::InvalidValueObject[] = "invalid value object";
const char JsonLdError::InvalidValueObjectValue[] = "invalid value object value";
const char JsonLdError::InvalidLanguageTaggedString[] = "invalid language-tagged string";
const char JsonLdError::InvalidLanguageTaggedValue[] = "invalid language-tagged value";
const char JsonLdError::InvalidTypedValue[] = "invalid typed value";
const char JsonLdError::InvalidSetOrListObject[] = "invalid set or list object";
const char JsonLdError::InvalidLanguageMapValue[] = "invalid language map value";
const char JsonLdError::CompactionToListOfLists[] = "compaction to list of lists";
const char JsonLdError::InvalidReversePropertyMap[] = "invalid reverse property map";
const char JsonLdError::InvalidReverseValue[] = "invalid @reverse value";
const char JsonLdError::InvalidReversePropertyValue[] = "invalid reverse property value";
const char JsonLdError::SyntaxError[] = "syntax error";
const char JsonLdError::NotImplemented[] = "not implemented";
const char JsonLdError::UnknownFormat[] = "unknown format";
const char JsonLdError::InvalidInput[] = "invalid input";
const char JsonLdError::ParseError[] = "parse error";
const char JsonLdError::UnknownError[] = "unknown error";
