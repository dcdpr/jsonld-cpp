#include "jsonld-cpp/CanonicalizationProcessor.h"
#include "jsonld-cpp/JsonLdProcessor.h"
#include "jsonld-cpp/JsonLdError.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/RDFCanonicalizationProcessor.h"
#include "jsonld-cpp/RemoteDocument.h"

using RDF::RDFDataset;


std::string CanonicalizationProcessor::canonicalize(const std::string& documentLocation, JsonLdOptions& options) {

    auto document = options.getDocumentLoader()->loadDocument(documentLocation);

    if(document->getContentType() == MediaType::json_ld()) {
        RDFDataset dataset = JsonLdProcessor::toRDF(documentLocation, options);
        return RDFCanonicalizationProcessor::canonicalize(dataset, options);
    }
    else if(document->getContentType() == MediaType::n_quads()) {
        RDFDataset dataset = document->getRDFContent();
        return RDFCanonicalizationProcessor::canonicalize(dataset, options);
    }
    else {
        std::stringstream ss;
        ss << "Unsupported content type: '" << document->getContentType()
           << "'. Supported content types for normalization are: "
           << MediaType::json_ld() << " and " << MediaType::n_quads();
        throw JsonLdError(JsonLdError::LoadingDocumentFailed, ss.str());
    }

}

