#ifndef LIBJSONLD_CPP_JSONLDOPTIONS_H
#define LIBJSONLD_CPP_JSONLDOPTIONS_H

// The JsonLdOptions type as specified in "JSON-LD-API specification":
// http://www.w3.org/TR/json-ld-api/#the-jsonldoptions-type

#include <string>
#include <sstream>
#include "DocumentLoader.h"
#include "JsonLdConsts.h"

using json = nlohmann::json;

class JsonLdOptions {
private:
    // Base options : http://www.w3.org/TR/json-ld-api/#idl-def-JsonLdOptions

    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-base
     */
    std::string base_;

    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-compactArrays
     */
    bool compactArrays_ = DEFAULT_COMPACT_ARRAYS;
    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-expandContext
     */
    json expandContext_;
    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-processingMode
     */
    std::string processingMode_ = JSON_LD_1_0;
    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-documentLoader
     */
    DocumentLoader documentLoader_;

    // Frame options : https://w3c.github.io/json-ld-framing/

    JsonLdConsts::Embed embed_ = JsonLdConsts::ONCE;
    bool explicit_ = false; // was null
    bool omitDefault_ = false; // was null
    bool omitGraph_ = false;
    bool frameExpansion_ = false;
    bool pruneBlankNodeIdentifiers_ = false;
    bool requireAll_ = false;
    bool allowContainerSetOnType_ = false;

    // RDF conversion options :
    // http://www.w3.org/TR/json-ld-api/#serialize-rdf-as-json-ld-algorithm

    bool useRdfType_ = false;
    bool useNativeTypes_ = false;
    bool produceGeneralizedRdf_ = false;

public:

    static constexpr const char JSON_LD_1_0[] = "json-ld-1.0";

    static constexpr const char JSON_LD_1_1[] = "json-ld-1.1";

    static constexpr bool DEFAULT_COMPACT_ARRAYS = true;

    /**
     * Constructs an instance of JsonLdOptions using the given base. Defaults to
     * empty base if none is given.
     *
     * @param base
     *            The base IRI for the document.
     */
    explicit JsonLdOptions(std::string base = "")
    : base_(std::move(base)) {
    }

    /**
     * Creates a shallow copy of this JsonLdOptions object.
     *
     * It will share the same DocumentLoader unless that is overridden, and
     * other mutable objects, so it isn't immutable.
     *
     * @return A copy of this JsonLdOptions object.
     */
// JsonLdOptions copy() {
//        final JsonLdOptions copy = new JsonLdOptions(base);
//
//        copy.setCompactArrays(compactArrays);
//        copy.setExpandContext(expandContext);
//        copy.setProcessingMode(processingMode);
//        copy.setDocumentLoader(documentLoader);
//        copy.setEmbed(embed);
//        copy.setExplicit(explicit);
//        copy.setOmitDefault(omitDefault);
//        copy.setOmitGraph(omitGraph);
//        copy.setFrameExpansion(frameExpansion);
//        copy.setPruneBlankNodeIdentifiers(pruneBlankNodeIdentifiers);
//        copy.setRequireAll(requireAll);
//        copy.setAllowContainerSetOnType(allowContainerSetOnType);
//        copy.setUseRdfType(useRdfType);
//        copy.setUseNativeTypes(useNativeTypes);
//        copy.setProduceGeneralizedRdf(produceGeneralizedRdf);
//        copy.format = format;
//        copy.useNamespaces = useNamespaces;
//        copy.outputForm = outputForm;
//
//        return copy;
//    }


    std::string getEmbed() {
        switch (embed_) {
            case JsonLdConsts::ALWAYS:
                return "@always";
            case JsonLdConsts::NEVER:
                return "@never";
            case JsonLdConsts::ONCE:
                return "@once";
            case JsonLdConsts::LINK:
                return "@link";
        }
    }

    JsonLdConsts::Embed getEmbedVal() {
        return embed_;
    }

    void setEmbed(bool embed) {
        this->embed_ = embed ? JsonLdConsts::ONCE : JsonLdConsts::NEVER;
    }

    void setEmbed(const std::string& embed) {
        if (embed == "@always") {
            this->embed_ = JsonLdConsts::ALWAYS;
        } else if (embed == "@never") {
            this->embed_ = JsonLdConsts::NEVER;
        } else if (embed == "@once") {
            this->embed_ = JsonLdConsts::ONCE;
        } else if (embed == "@link") {
            this->embed_ = JsonLdConsts::LINK;
        } else {
            std::stringstream ss;
            ss << "Error: Invalid embed value: [" << embed << "]";
            throw std::runtime_error(ss.str());
            //throw new JsonLdError(JsonLdError.Error.INVALID_EMBED_VALUE);
        }
    }

    void setEmbed(JsonLdConsts::Embed embed)  {
        this->embed_ = embed;
    }

    bool getExplicit() {
        return explicit_;
    }

    void setExplicit(bool explicitFlag) {
        this->explicit_ = explicitFlag;
    }

    bool getOmitDefault() {
        return omitDefault_;
    }

    void setOmitDefault(bool omitDefault) {
        this->omitDefault_ = omitDefault;
    }

    bool getFrameExpansion() {
        return frameExpansion_;
    }

    void setFrameExpansion(bool frameExpansion) {
        this->frameExpansion_ = frameExpansion;
    }

    bool getOmitGraph() {
        return omitGraph_;
    }

    void setOmitGraph(bool omitGraph) {
        this->omitGraph_ = omitGraph;
    }

    bool getPruneBlankNodeIdentifiers() {
        return pruneBlankNodeIdentifiers_;
    }

    void setPruneBlankNodeIdentifiers(bool pruneBlankNodeIdentifiers) {
        this->pruneBlankNodeIdentifiers_ = pruneBlankNodeIdentifiers;
    }

    bool getRequireAll() {
        return requireAll_;
    }

    void setRequireAll(bool requireAll) {
        this->requireAll_ = requireAll;
    }

    bool getAllowContainerSetOnType() {
        return allowContainerSetOnType_;
    }

    void setAllowContainerSetOnType(bool allowContainerSetOnType) {
        this->allowContainerSetOnType_ = allowContainerSetOnType;
    }

    bool getCompactArrays() {
        return compactArrays_;
    }

    void setCompactArrays(bool compactArrays) {
        this->compactArrays_ = compactArrays;
    }

    json getExpandContext() {
        return expandContext_;
    }

    void setExpandContext(json expandContext) {
        this->expandContext_ = std::move(expandContext);
    }

    std::string getProcessingMode() {
        return processingMode_;
    }

    void setProcessingMode(const std::string& processingMode) {
        this->processingMode_ = processingMode;
        if (processingMode == JSON_LD_1_1) {
            omitGraph_ = true;
            pruneBlankNodeIdentifiers_ = true;
            allowContainerSetOnType_ = true;
        }
    }

    std::string getBase() {
        return base_;
    }

    void setBase(std::string base) {
        this->base_ = std::move(base);
    }

    bool getUseRdfType() {
        return useRdfType_;
    }

    void setUseRdfType(bool useRdfType) {
        this->useRdfType_ = useRdfType;
    }

    bool getUseNativeTypes() {
        return useNativeTypes_;
    }

    void setUseNativeTypes(bool useNativeTypes) {
        this->useNativeTypes_ = useNativeTypes;
    }

    bool getProduceGeneralizedRdf() {
        return produceGeneralizedRdf_;
    }

    void setProduceGeneralizedRdf(bool produceGeneralizedRdf) {
        this->produceGeneralizedRdf_ = produceGeneralizedRdf;
    }

    DocumentLoader getDocumentLoader() {
        return documentLoader_;
    }

    void setDocumentLoader(DocumentLoader documentLoader) {
        this->documentLoader_ = std::move(documentLoader);
    }

//    // TODO: THE FOLLOWING ONLY EXIST SO I DON'T HAVE TO DELETE A LOT OF CODE,
//    // REMOVE IT WHEN DONE
//    std::string format;
//    bool useNamespaces = false;
//    std::string outputForm;

};

#endif //LIBJSONLD_CPP_JSONLDOPTIONS_H
