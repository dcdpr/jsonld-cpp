#ifndef LIBJSONLD_CPP_JSONLDOPTIONS_H
#define LIBJSONLD_CPP_JSONLDOPTIONS_H

// The JsonLdOptions type as specified in "JSON-LD-API specification":
// http://www.w3.org/TR/json-ld-api/#the-jsonldoptions-type

#include "DocumentLoader.h"
#include "JsonLdConsts.h"
#include <string>
#include <sstream>

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
    nlohmann::json expandContext_;
    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-processingMode
     */
    std::string processingMode_ = JSON_LD_1_1;
    /**
     * http://www.w3.org/TR/json-ld-api/#widl-JsonLdOptions-documentLoader
     */
    std::unique_ptr<DocumentLoader> documentLoader_;

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

    explicit JsonLdOptions(std::unique_ptr<DocumentLoader> documentLoader)
            : documentLoader_(std::move(documentLoader)) {
    }

    JsonLdOptions(const JsonLdOptions& other) {
        base_ = other.base_;
        compactArrays_ = other.compactArrays_;
        expandContext_ = other.expandContext_;
        processingMode_ = other.processingMode_;
        if(other.documentLoader_)
            documentLoader_.reset( other.documentLoader_->clone() );

        embed_ = other.embed_;
        explicit_ = other.explicit_;
        omitDefault_ = other.omitDefault_;
        omitGraph_ = other.omitGraph_;
        frameExpansion_ = other.frameExpansion_;
        pruneBlankNodeIdentifiers_ = other.pruneBlankNodeIdentifiers_;
        requireAll_ = other.requireAll_;
        allowContainerSetOnType_ = other.allowContainerSetOnType_;

        useRdfType_ = other.useRdfType_;
        useNativeTypes_ = other.useNativeTypes_;
        produceGeneralizedRdf_ = other.produceGeneralizedRdf_;
    }

    JsonLdOptions & operator=(const JsonLdOptions& other) {
        documentLoader_.reset( other.documentLoader_->clone() );
        return *this;
    }

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

    nlohmann::json getExpandContext() {
        return expandContext_;
    }

    void setExpandContext(nlohmann::json expandContext) {
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

    DocumentLoader * getDocumentLoader() {
        return documentLoader_.get();
    }

    void setDocumentLoader(std::unique_ptr<DocumentLoader> documentLoader) {
        this->documentLoader_ = std::move(documentLoader);
    }

};

#endif //LIBJSONLD_CPP_JSONLDOPTIONS_H
