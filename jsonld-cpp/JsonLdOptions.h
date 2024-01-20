#ifndef LIBJSONLD_CPP_JSONLDOPTIONS_H
#define LIBJSONLD_CPP_JSONLDOPTIONS_H

// The JsonLdOptions type as specified in "JSON-LD-API specification":
// https://www.w3.org/TR/json-ld-api/#the-jsonldoptions-type

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/DocumentLoader.h"
#include "jsonld-cpp/JsonLdConsts.h"
#include <string>
#include <sstream>

class JsonLdOptions {
private:
    // ///////////////////////////////////////////////////////////////////////
    // Base options : https://www.w3.org/TR/json-ld11-api/#the-jsonldoptions-type

    /**
     * The base IRI to use when expanding or compacting the document. If set, this
     * overrides the input document's IRI.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-base
     */
    std::string base_;

    /**
     * If set to true, the JSON-LD processor replaces arrays with just one element with
     * that element during compaction. If set to false, all arrays will remain arrays even
     * if they have just one element.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-compactarrays
     */
    bool compactArrays_ = JsonLdConsts::DEFAULT_COMPACT_ARRAYS;

    /**
     * Determines if IRIs are compacted relative to the base option or document location
     * when compacting.
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldoptions-compacttorelative
     */
    bool compactToRelative_ = true;

    /**
     * The callback of the loader to be used to retrieve remote documents and
     * contexts, implementing the LoadDocumentCallback. If specified, it is used to
     * retrieve remote documents and contexts; otherwise, if not specified, the
     * processor's built-in loader is used.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-documentloader
     * Note: jsonld-cpp doesn't support callbacks yet. See DocumentLoader.h for its API.
     */
    std::unique_ptr<DocumentLoader> documentLoader_;

    /**
     * A context that is used to initialize the active context when expanding a document.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-expandcontext
     */
    nlohmann::json expandContext_;

    /**
     * If set to true, when extracting JSON-LD script elements from HTML, unless a specific
     * fragment identifier is targeted, extracts all encountered JSON-LD script elements
     * using an array form, if necessary.
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldoptions-extractallscripts
     */
    bool extractAllScripts_ = false;

    /**
     * Enables special frame processing rules for the Expansion Algorithm.
     * Also Enables special rules for the Serialize RDF as JSON-LD Algorithm to use JSON-LD
     * native types as values, where possible.
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldoptions-frameexpansion
     */
    bool frameExpansion_ = false;

    /**
     * If set to true, certain algorithm processing steps where indicated are ordered
     * lexicographically. If false, order is not considered in processing.
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldoptions-ordered
     */
    bool ordered_ = false;

    /**
     * Sets the processing mode. If set to json-ld-1.0 or json-ld-1.1, the implementation
     * must produce exactly the same results as the algorithms defined in this
     * specification. If set to another value, the JSON-LD processor is allowed to extend
     * or modify the algorithms defined in this specification to enable application-specific
     * optimizations. The definition of such optimizations is beyond the scope of this
     * specification and thus not defined. Consequently, different implementations may
     * implement different optimizations. Developers must not define modes beginning with
     * json-ld as they are reserved for future versions of this specification.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-processingmode
     */
    std::string processingMode_ = JsonLdConsts::JSON_LD_1_1;

    /**
     * If set to true, the JSON-LD processor may emit blank nodes for triple
     * predicates, otherwise they will be omitted. Generalized RDF Datasets are
     * defined in https://www.w3.org/TR/rdf11-concepts/.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-producegeneralizedrdf
     */
    bool produceGeneralizedRdf_ = false;

    /**
     * Determines how value objects containing a base direction are transformed to and from RDF.
     * https://www.w3.org/TR/json-ld11-api/#dom-jsonldoptions-rdfdirection
     */
    std::string rdfDirection_;

    /**
     * Causes the Serialize RDF as JSON-LD Algorithm to use native JSON values in value
     * objects avoiding the need for an explicitly \@type.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-usenativetypes
     */
    bool useNativeTypes_ = false;

    /**
     * Enables special rules for the Serialize RDF as JSON-LD Algorithm causing rdf:type
     * properties to be kept as IRIs in the output, rather than use \@type.
     * https://www.w3.org/TR/json-ld-api/#dom-jsonldoptions-userdftype
     */
    bool useRdfType_ = false;

    std::string hashAlgorithm_;

public:

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
        compactToRelative_ = other.compactToRelative_;
        if(other.documentLoader_)
            documentLoader_.reset( other.documentLoader_->clone() );
        expandContext_ = other.expandContext_;
        extractAllScripts_ = other.extractAllScripts_;
        frameExpansion_ = other.frameExpansion_;
        hashAlgorithm_ = other.hashAlgorithm_;
        ordered_ = other.ordered_;
        processingMode_ = other.processingMode_;
        produceGeneralizedRdf_ = other.produceGeneralizedRdf_;
        rdfDirection_ = other.rdfDirection_;
        useNativeTypes_ = other.useNativeTypes_;
        useRdfType_ = other.useRdfType_;
    }

    JsonLdOptions & operator=(const JsonLdOptions& other) {
        documentLoader_.reset( other.documentLoader_->clone() );
        return *this;
    }

    bool isFrameExpansion() const;

    void setFrameExpansion(bool frameExpansion);

    bool isOrdered() const;

    void setOrdered(bool ordered);

    bool getCompactArrays() const {
        return compactArrays_;
    }

    void setCompactArrays(bool compactArrays) {
        this->compactArrays_ = compactArrays;
    }

    bool getCompactToRelative() const {
        return compactToRelative_;
    }

    void setCompactToRelative(bool compactToRelative) {
        this->compactToRelative_ = compactToRelative;
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
//        if (processingMode == JsonLdConsts::JSON_LD_1_1) {
//            omitGraph_ = true;
//        }
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

    std::string getRdfDirection() const {
        return rdfDirection_;
    }

    void setRdfDirection(const std::string& rdfDirection) {
        this->rdfDirection_ = rdfDirection;
    }

    DocumentLoader * getDocumentLoader() const {
        return documentLoader_.get();
    }

    void setDocumentLoader(std::unique_ptr<DocumentLoader> documentLoader) {
        this->documentLoader_ = std::move(documentLoader);
    }

    const std::string &getHashAlgorithm() const;

    void setHashAlgorithm(const std::string &hashAlgorithm);

};

#endif //LIBJSONLD_CPP_JSONLDOPTIONS_H
