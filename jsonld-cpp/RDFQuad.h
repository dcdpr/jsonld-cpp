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

        RDFQuad(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object,
                std::string * graph);

        RDFQuad(const std::string& subject, const std::string& predicate, const std::string& object, std::string * graph);

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

    };

    bool operator==(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator!=(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator<(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator<=(const RDFQuad &lhs, const RDFQuad &rhs);
    bool operator>=(const RDFQuad &lhs, const RDFQuad &rhs);

    struct QuadPtrLess  {
        bool operator()(const std::shared_ptr<RDFQuad> & lhs, const std::shared_ptr<RDFQuad> & rhs) const {
            return *lhs < *rhs ;
        }
    };

}

#endif //LIBJSONLD_CPP_RDFQUAD_H
