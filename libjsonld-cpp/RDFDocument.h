#ifndef LIBJSONLD_CPP_RDFDOCUMENT_H
#define LIBJSONLD_CPP_RDFDOCUMENT_H


#include "RemoteDocument.h"

class RDFDocument : public RemoteDocument {
private:
    MediaType contentType;
    std::string documentUrl;
    std::string document;
    std::string contextUrl;

    RDFDocument(MediaType contentType, std::string document);

public:

    static RDFDocument of(const MediaType& contentType, std::istream &in);

    static bool accepts(const MediaType& contentType);
    static void assertContentType(const MediaType& contentType);

    MediaType getContentType() const override;
    const std::string &getContextUrl() const override;
    const std::string &getDocumentUrl() const override;
    const nlohmann::json &getJSONContent() const override;
    const std::string & getRDFContent() const override;

    void setContextUrl(const std::string &url) override;
    void setDocumentUrl(const std::string &url) override;
};


#endif //LIBJSONLD_CPP_RDFDOCUMENT_H
