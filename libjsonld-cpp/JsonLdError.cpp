#include "JsonLdError.h"

const auto JsonLdError::LoadingDocumentFailed = JsonLdError::Error("loading document failed");
const auto JsonLdError::ListOfLists = JsonLdError::Error("list of lists");
const auto JsonLdError::InvalidIndexValue = JsonLdError::Error ("invalid @index value");
const auto JsonLdError::ConflictingIndexes = JsonLdError::Error ("conflicting indexes");
const auto JsonLdError::InvalidIdValue = JsonLdError::Error("invalid @id value");
const auto JsonLdError::InvalidLocalContext = JsonLdError::Error ("invalid local context");
const auto JsonLdError::MultipleContextLinkHeaders = JsonLdError::Error ("multiple context link headers");
const auto JsonLdError::LoadingRemoteContextFailed = JsonLdError::Error ("loading remote context failed");
const auto JsonLdError::InvalidRemoteContext = JsonLdError::Error ("invalid remote context");
const auto JsonLdError::RecursiveContextInclusion = JsonLdError::Error ("recursive context inclusion");
const auto JsonLdError::InvalidBaseIri = JsonLdError::Error("invalid base IRI");
const auto JsonLdError::InvalidVocabMapping = JsonLdError::Error ("invalid vocab mapping");
const auto JsonLdError::InvalidDefaultLanguage = JsonLdError::Error ("invalid default language");
const auto JsonLdError::KeywordRedefinition = JsonLdError::Error ("keyword redefinition");
const auto JsonLdError::InvalidTermDefinition = JsonLdError::Error ("invalid term definition");
const auto JsonLdError::InvalidReverseProperty = JsonLdError::Error ("invalid reverse property");
const auto JsonLdError::InvalidIriMapping = JsonLdError::Error ("invalid IRI mapping");
const auto JsonLdError::CyclicIriMapping = JsonLdError::Error ("cyclic IRI mapping");
const auto JsonLdError::InvalidKeywordAlias = JsonLdError::Error ("invalid keyword alias");
const auto JsonLdError::InvalidTypeMapping = JsonLdError::Error ("invalid type mapping");
const auto JsonLdError::InvalidLanguageMapping = JsonLdError::Error ("invalid language mapping");
const auto JsonLdError::CollidingKeywords = JsonLdError::Error ("colliding keywords");
const auto JsonLdError::InvalidContainerMapping = JsonLdError::Error ("invalid container mapping");
const auto JsonLdError::InvalidTypeValue = JsonLdError::Error ("invalid type value");
const auto JsonLdError::InvalidValueObject = JsonLdError::Error ("invalid value object");
const auto JsonLdError::InvalidValueObjectValue = JsonLdError::Error ("invalid value object value");
const auto JsonLdError::InvalidLanguageTaggedString = JsonLdError::Error ("invalid language-tagged string");
const auto JsonLdError::InvalidLanguageTaggedValue = JsonLdError::Error ("invalid language-tagged value");
const auto JsonLdError::InvalidTypedValue = JsonLdError::Error ("invalid typed value");
const auto JsonLdError::InvalidSetOrListObject = JsonLdError::Error ("invalid set or list object");
const auto JsonLdError::InvalidLanguageMapValue = JsonLdError::Error ("invalid language map value");
const auto JsonLdError::CompactionToListOfLists = JsonLdError::Error ("compaction to list of lists");
const auto JsonLdError::InvalidReversePropertyMap = JsonLdError::Error ("invalid reverse property map");
const auto JsonLdError::InvalidReverseValue = JsonLdError::Error ("invalid @reverse value");
const auto JsonLdError::InvalidReversePropertyValue = JsonLdError::Error ("invalid reverse property value");
const auto JsonLdError::SyntaxError = JsonLdError::Error("syntax error");
const auto JsonLdError::NotImplemented = JsonLdError::Error("not implemented");
const auto JsonLdError::UnknownFormat = JsonLdError::Error("unknown format");
const auto JsonLdError::InvalidInput = JsonLdError::Error("invalid input");
const auto JsonLdError::ParseError = JsonLdError::Error("parse error");
const auto JsonLdError::UnknownError = JsonLdError::Error("unknown error");

const JsonLdError::Error &JsonLdError::getType() const {
    return type;
}
