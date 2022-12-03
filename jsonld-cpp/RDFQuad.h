#ifndef LIBJSONLD_CPP_RDFQUAD_H
#define LIBJSONLD_CPP_RDFQUAD_H

#include "jsonld-cpp/RDFNode.h"
#include <memory>
#include <string>


namespace RDF {

    class RDFQuadImpl;

    class RDFQuad {
    private:
        std::shared_ptr<RDFQuadImpl> pimpl_;

        std::shared_ptr<Node> graph;

        void setGraph(std::shared_ptr<Node> igraph)  { graph = std::move(igraph); }

    public:

        RDFQuad(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject,
                std::string * igraph);

        RDFQuad(const std::string& isubject, const std::string& ipredicate, const std::string& iobject, std::string * igraph);

        RDFQuad(const RDFQuad &rhs);
        RDFQuad(RDFQuad&& rhs) noexcept;

        virtual ~RDFQuad();

        RDFQuad& operator= (const RDFQuad &rhs);
        RDFQuad& operator= (RDFQuad&& rhs) noexcept;

        std::shared_ptr<Node> getPredicate() const;
        std::shared_ptr<Node> getObject() const;
        std::shared_ptr<Node> getSubject() const;
        std::shared_ptr<Node> getGraph() const;

        std::string toString() const;

        friend bool operator==(const RDFQuad &lhs, const RDFQuad &rhs);

    };

    bool operator==(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator!=(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator<(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator<=(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>=(const RDFQuad &lhs, const RDFQuad &rhs);

    struct QuadPtrLess :
            public std::binary_function<const std::shared_ptr<RDFQuad>, const std::shared_ptr<RDFQuad>, bool> {
        bool operator()(const std::shared_ptr<RDFQuad> & lhs, const std::shared_ptr<RDFQuad> & rhs) const {
            return *lhs < *rhs ;
        }
    };

}

#endif //LIBJSONLD_CPP_RDFQUAD_H
