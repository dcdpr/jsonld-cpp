#ifndef LIBJSONLD_CPP_JSONLDOPTIONS_H
#define LIBJSONLD_CPP_JSONLDOPTIONS_H

// The JsonLdOptions type as specified in "JSON-LD-API specification":
// https://www.w3.org/TR/json-ld-api/#the-jsonldoptions-type

#include "jsonld-cpp/DocumentLoader.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include <string>
#include <sstream>

class JsonLdOptions {
private:
    // ///////////////////////////////////////////////////////////////////////
    // Base options : https://www.w3.org/TR/json-ld-api/#idl-def-JsonLdOptions

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-base
     */
    std::string base_;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-compactarrays
     */
    bool compactArrays_ = DEFAULT_COMPACT_ARRAYS;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-expandcontext
     */
    nlohmann::json expandContext_;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-processingmode
     */
    std::string processingMode_ = JSON_LD_1_1;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-documentloader
     */
    std::unique_ptr<DocumentLoader> documentLoader_;

    // //////////////////////////////////////////////////////////
    // Framing options : https://www.w3.org/TR/json-ld11-framing/

    /**
     * https://www.w3.org/TR/json-ld11-framing/#dom-jsonldoptions-embed
     */
    JsonLdConsts::Embed embed_ = JsonLdConsts::ONCE;

    /**
     * https://www.w3.org/TR/json-ld11-framing/#dom-jsonldoptions-explicit
     */
    bool explicit_ = false;

    /**
     * https://www.w3.org/TR/json-ld11-framing/#dom-jsonldoptions-omitdefault
     */
    bool omitDefault_ = false;

    /**
     * https://www.w3.org/TR/json-ld11-framing/#dom-jsonldoptions-omitgraph
     */
    bool omitGraph_ = false;
    /**
     * https://www.w3.org/TR/json-ld11-framing/#dom-jsonldoptions-requireall
     */
    bool requireAll_ = false;

    // todo: frameDefault, ordered?

    // ///////////////////////////////////////////////////////////////////////
    // RDF conversion options :
    // https://www.w3.org/TR/json-ld-api/#serialize-rdf-as-json-ld-algorithm

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-userdftype
     */
    bool useRdfType_ = false;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-usenativetypes
     */
    bool useNativeTypes_ = false;

    /**
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-producegeneralizedrdf
     */
    bool produceGeneralizedRdf_ = false;

public:

    static constexpr const char JSON_LD_1_0[] = "json-ld-1.0";

    static constexpr const char JSON_LD_1_1[] = "json-ld-1.1";

    static constexpr bool DEFAULT_COMPACT_ARRAYS = true;

    /**
     * Constructs an instance of JsonLdOptions using the given base IRI. Defaults to
     * empty base IRI if none is given.
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
        requireAll_ = other.requireAll_;

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

    bool getExplicit() const {
        return explicit_;
    }

    void setExplicit(bool explicitFlag) {
        this->explicit_ = explicitFlag;
    }

    bool getOmitDefault() const {
        return omitDefault_;
    }

    void setOmitDefault(bool omitDefault) {
        this->omitDefault_ = omitDefault;
    }

    bool getOmitGraph() const {
        return omitGraph_;
    }

    void setOmitGraph(bool omitGraph) {
        this->omitGraph_ = omitGraph;
    }

    bool getRequireAll() const {
        return requireAll_;
    }

    void setRequireAll(bool requireAll) {
        this->requireAll_ = requireAll;
    }

    bool getCompactArrays() const {
        return compactArrays_;
    }

    void setCompactArrays(bool compactArrays) {
        this->compactArrays_ = compactArrays;
    }

    nlohmann::json getExpandContext() const {
        return expandContext_;
    }

    void setExpandContext(nlohmann::json expandContext) {
        this->expandContext_ = std::move(expandContext);
    }

    std::string getProcessingMode() const {
        return processingMode_;
    }

    void setProcessingMode(const std::string& processingMode) {
        this->processingMode_ = processingMode;
        if (processingMode == JSON_LD_1_1) {
            omitGraph_ = true;
        }
    }

    std::string getBase() const {
        return base_;
    }

    void setBase(std::string base) {
        this->base_ = std::move(base);
    }

    bool getUseRdfType() const {
        return useRdfType_;
    }

    void setUseRdfType(bool useRdfType) {
        this->useRdfType_ = useRdfType;
    }

    bool getUseNativeTypes() const {
        return useNativeTypes_;
    }

    void setUseNativeTypes(bool useNativeTypes) {
        this->useNativeTypes_ = useNativeTypes;
    }

    bool getProduceGeneralizedRdf() const {
        return produceGeneralizedRdf_;
    }

    void setProduceGeneralizedRdf(bool produceGeneralizedRdf) {
        this->produceGeneralizedRdf_ = produceGeneralizedRdf;
    }

    DocumentLoader * getDocumentLoader() const {
        return documentLoader_.get();
    }

    void setDocumentLoader(std::unique_ptr<DocumentLoader> documentLoader) {
        this->documentLoader_ = std::move(documentLoader);
    }

};

#endif //LIBJSONLD_CPP_JSONLDOPTIONS_H
