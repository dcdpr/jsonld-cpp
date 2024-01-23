#ifndef LIBJSONLD_CPP_RDFDOCUMENT_H
#define LIBJSONLD_CPP_RDFDOCUMENT_H

#include "jsonld-cpp/RemoteDocument.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/MediaType.h"


class RDFDocument : public RemoteDocument {
private:
    MediaType contentType;
    std::string documentUrl;
    RDF::RDFDataset document;
    std::string contextUrl;

    RDFDocument(MediaType contentType, RDF::RDFDataset document);

public:

    static RDFDocument of(const MediaType& contentType, std::istream &in);

    static bool accepts(const MediaType& contentType);
    static void assertContentType(const MediaType& contentType);

    MediaType getContentType() const override;
    const std::string &getContextUrl() const override;
    const std::string &getDocumentUrl() const override;
    const nlohmann::ordered_json &getJSONContent() const override;
    const RDF::RDFDataset & getRDFContent() const override;

    void setContextUrl(const std::string &url) override;
    void setDocumentUrl(const std::string &url) override;
};


#endif //LIBJSONLD_CPP_RDFDOCUMENT_H
