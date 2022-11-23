#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/NQuadsSerialization.h"

using nlohmann::json;

namespace RDF {

    class RDFQuadImpl : private RDFTriple {
    public:
        RDFQuadImpl(std::shared_ptr<Node> &isubject, std::shared_ptr<Node> &ipredicate, std::shared_ptr<Node> &iobject)
                : RDFTriple(isubject, ipredicate, iobject) {}

        RDFQuadImpl(const std::string &isubject, const std::string &ipredicate, const std::string &iobject)
                : RDFTriple(isubject, ipredicate, iobject) {}

        explicit RDFQuadImpl(const RDFTriple &rhs) : RDFTriple(rhs) {}

        explicit RDFQuadImpl(RDFTriple &&rhs) : RDFTriple(rhs) {}

        std::shared_ptr<Node> getSubject() const override {
            return RDFTriple::getSubject();
        }

        std::shared_ptr<Node> getPredicate() const override {
            return RDFTriple::getPredicate();
        }

        std::shared_ptr<Node> getObject() const override {
            return RDFTriple::getObject();
        }

    };

    RDFQuad::RDFQuad(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject,
                     std::string *igraph)
            : pimpl_(new RDFQuadImpl(isubject, ipredicate, iobject))
    {
        if (igraph != nullptr && *igraph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*igraph) ?
            setGraph(std::make_shared<BlankNode>(*igraph)) :
            setGraph(std::make_shared<IRI>(*igraph));
        }
    }

    RDFQuad::RDFQuad(const std::string& isubject, const std::string& ipredicate, const std::string& iobject, std::string *igraph)
            : pimpl_(new RDFQuadImpl(isubject, ipredicate, iobject))
    {
        if (igraph != nullptr && *igraph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*igraph) ?
                setGraph(std::make_shared<BlankNode>(*igraph)) :
                setGraph(std::make_shared<IRI>(*igraph));
        }
    }

    std::shared_ptr<Node> RDFQuad::getSubject() const {
        return pimpl_->getSubject();
    }

    std::shared_ptr<Node> RDFQuad::getPredicate() const {
        return pimpl_->getPredicate();
    }

    std::shared_ptr<Node> RDFQuad::getObject() const {
        return pimpl_->getObject();
    }

    std::shared_ptr<Node> RDFQuad::getGraph() const {
        return graph;
    }

    std::string RDFQuad::toString() const {
        return NQuadsSerialization::toNQuad(*this);
    }

    RDFQuad::RDFQuad(const RDFQuad &rhs) {
        pimpl_ = rhs.pimpl_;

        if(rhs.graph && rhs.graph->isBlankNode())
            graph = std::make_shared<BlankNode>(rhs.graph->getValue());
        else if(rhs.graph && rhs.graph->isIRI())
            graph = std::make_shared<IRI>(rhs.graph->getValue());
    }

    RDFQuad::RDFQuad(RDFQuad&& rhs) noexcept {
        pimpl_ = std::move(rhs.pimpl_);
        graph = std::move(rhs.graph);
    }

    RDFQuad::~RDFQuad() {

    }

    RDFQuad & RDFQuad::operator=(const RDFQuad &rhs) {
        if (&rhs != this) {
            pimpl_ = rhs.pimpl_;
            graph = std::shared_ptr<Node>{rhs.graph};
        }
        return *this;
    }

    RDFQuad& RDFQuad::operator= (RDFQuad&& rhs) noexcept {
        if (&rhs != this) {
            pimpl_ = std::move(rhs.pimpl_);
            graph = std::shared_ptr<Node>{rhs.graph};
            rhs.graph = nullptr;
        }
        return *this;
    }

    bool operator==(const RDFQuad &lhs, const RDFQuad &rhs) {
        NodePtrEquals equals;
        return
                equals(lhs.getSubject(), rhs.getSubject()) &&
                equals(lhs.getPredicate(), rhs.getPredicate()) &&
                equals(lhs.getObject(), rhs.getObject()) &&
                equals(lhs.getGraph(), rhs.getGraph());
    }

    bool operator!=(const RDFQuad &lhs, const RDFQuad &rhs) {
        return !(rhs == lhs);
    }

    bool operator<(const RDFQuad &lhs, const RDFQuad &rhs) {
        NodePtrLess less;
        NodePtrEquals equals;
        if(!equals(lhs.getGraph(), rhs.getGraph()))
            return less(lhs.getGraph(), rhs.getGraph());
        if(!equals(lhs.getSubject(), rhs.getSubject()))
            return less(lhs.getSubject(), rhs.getSubject());
        if(!equals(lhs.getPredicate(), rhs.getPredicate()))
            return less(lhs.getPredicate(), rhs.getPredicate());

        return less(lhs.getObject(), rhs.getObject());
    }

    bool operator>(const RDFQuad &lhs, const RDFQuad &rhs) {
        return rhs < lhs;
    }

    bool operator<=(const RDFQuad &lhs, const RDFQuad &rhs) {
        return !(rhs < lhs);
    }

    bool operator>=(const RDFQuad &lhs, const RDFQuad &rhs) {
        return !(lhs < rhs);
    }

}
