#ifndef LIBJSONLD_CPP_JSONDOCUMENT_H
#define LIBJSONLD_CPP_JSONDOCUMENT_H


#include "RemoteDocument.h"

class JSONDocument : public RemoteDocument {
private:
    MediaType contentType;
    std::string documentUrl;
    nlohmann::json document;
    std::string contextUrl;

    JSONDocument(MediaType contentType, nlohmann::json document);

public:

    static JSONDocument of(const MediaType& contentType, std::istream &in);

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


#endif //LIBJSONLD_CPP_JSONDOCUMENT_H
