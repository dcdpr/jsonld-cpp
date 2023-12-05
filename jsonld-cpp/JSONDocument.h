#ifndef LIBJSONLD_CPP_JSONDOCUMENT_H
#define LIBJSONLD_CPP_JSONDOCUMENT_H

#include "jsonld-cpp/RemoteDocument.h"

namespace RDF {
    class RDFDataset;
}

class JSONDocument : public RemoteDocument {
private:
    MediaType contentType;
    std::string documentUrl;
    nlohmann::json document;
    std::string contextUrl;

    JSONDocument(MediaType contentType, nlohmann::json document, std::string documentUrl);

public:

    static JSONDocument of(const MediaType& contentType, std::istream &in, const std::string & documentUrl);

    static bool accepts(const MediaType& contentType);
    static void assertContentType(const MediaType& contentType);

    MediaType getContentType() const override;
    const std::string &getContextUrl() const override;
    const std::string &getDocumentUrl() const override;
    const nlohmann::json &getJSONContent() const override;
    const RDF::RDFDataset & getRDFContent() const override;

    void setContextUrl(const std::string &url) override;
    void setDocumentUrl(const std::string &url) override;
};


#endif //LIBJSONLD_CPP_JSONDOCUMENT_H
