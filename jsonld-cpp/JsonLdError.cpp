#include "jsonld-cpp/JsonLdError.h"

using json = nlohmann::ordered_json;

JsonLdError::JsonLdError(const std::string& type, const json& detail)
        : std::runtime_error(type + ": " + detail.get<std::string>())
{
}

JsonLdError::JsonLdError(const std::string& type)
        : std::runtime_error(type)
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
const char JsonLdError::IllegalArgument[] = "illegal argument";
const char JsonLdError::InvalidVersionValue[] = "invalid @version value";
const char JsonLdError::ProcessingModeConflict[] = "processing mode conflict";
const char JsonLdError::InvalidProtectedValue[] = "invalid @protected value";
const char JsonLdError::InvalidScopedContext[] = "invalid scoped context";
const char JsonLdError::InvalidBaseDirection[] = "invalid base direction";
const char JsonLdError::InvalidNestValue[] = "invalid @nest value";
const char JsonLdError::InvalidPrefixValue[] = "invalid @prefix value";
const char JsonLdError::InvalidContextNullification[] = "invalid context nullification";
const char JsonLdError::ContextOverflow[] = "context overflow";
const char JsonLdError::InvalidContextEntry[] = "invalid context entry";
const char JsonLdError::InvalidImportValue[] = "invalid @import value";
const char JsonLdError::InvalidPropagateValue[] = "invalid @propagate value";
const char JsonLdError::InvalidIncludedValue[] = "invalid @included value";
const char JsonLdError::ProtectedTermRedefinition[] = "protected term redefinition";
