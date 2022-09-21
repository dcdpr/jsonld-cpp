#ifndef LIBJSONLD_CPP_REMOTEDOCUMENT_H
#define LIBJSONLD_CPP_REMOTEDOCUMENT_H

#include "jsonld-cpp/jsoninc.h"
#include "jsonld-cpp/MediaType.h"
#include <string>

namespace RDF {
    class RDFDataset;
}


class RemoteDocument {

public:
    RemoteDocument() = default;
    RemoteDocument(const RemoteDocument&) = default;
    RemoteDocument& operator=(const RemoteDocument&) = default;

    virtual ~RemoteDocument();

    virtual MediaType getContentType() const = 0;
    virtual const std::string &getContextUrl() const = 0;
    virtual const std::string &getDocumentUrl() const = 0;
    virtual const nlohmann::json &getJSONContent() const = 0;
    virtual const RDF::RDFDataset & getRDFContent() const = 0;

    virtual void setContextUrl(const std::string &url) = 0;
    virtual void setDocumentUrl(const std::string &url) = 0;

};

#endif //LIBJSONLD_CPP_REMOTEDOCUMENT_H
