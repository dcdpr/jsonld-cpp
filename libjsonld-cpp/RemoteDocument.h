#ifndef LIBJSONLD_CPP_REMOTEDOCUMENT_H
#define LIBJSONLD_CPP_REMOTEDOCUMENT_H

#include "jsoninc.h"
#include "MediaType.h"
//#include "RDFDataset.h"
#include <string>

namespace RDF {
    class RDFDataset;
}


class RemoteDocument {

public:
    RemoteDocument() = default;                                  // constructor
    RemoteDocument(const RemoteDocument&) = default;             // copy constructor
    RemoteDocument& operator=(const RemoteDocument&) = default;  // copy assignment

    virtual ~RemoteDocument();

    virtual MediaType getContentType() const = 0;
    virtual const std::string &getContextUrl() const = 0;
    virtual const std::string &getDocumentUrl() const = 0;
    virtual const nlohmann::json &getJSONContent() const = 0;
    virtual const std::string & getRDFContent() const = 0;

    virtual void setContextUrl(const std::string &url) = 0;
    virtual void setDocumentUrl(const std::string &url) = 0;

};

#endif //LIBJSONLD_CPP_REMOTEDOCUMENT_H
