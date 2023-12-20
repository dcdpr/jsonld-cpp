#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/NQuadsSerialization.h"

using nlohmann::json;

namespace RDF {

    class RDFQuadImpl : private RDFTriple {
    public:
        RDFQuadImpl(std::shared_ptr<Node> &subject, std::shared_ptr<Node> &predicate, std::shared_ptr<Node> &object)
                : RDFTriple(subject, predicate, object) {}

        RDFQuadImpl(const std::string &subject, const std::string &predicate, const std::string &object)
                : RDFTriple(subject, predicate, object) {}

        explicit RDFQuadImpl(const RDFTriple &rhs) : RDFTriple(rhs) {}

        explicit RDFQuadImpl(RDFTriple &&rhs) : RDFTriple(rhs) {}

        std::shared_ptr<Node> getSubject() const override;

        std::shared_ptr<Node> getPredicate() const override;

        std::shared_ptr<Node> getObject() const override;

    };

    std::shared_ptr<Node> RDFQuadImpl::getSubject() const {
        return RDFTriple::getSubject();
    }

    std::shared_ptr<Node> RDFQuadImpl::getPredicate() const {
        return RDFTriple::getPredicate();
    }

    std::shared_ptr<Node> RDFQuadImpl::getObject() const {
        return RDFTriple::getObject();
    }

    RDFQuad::RDFQuad(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object,
                     std::string *graph)
            : pimpl_(new RDFQuadImpl(subject, predicate, object))
    {
        if (graph != nullptr && *graph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*graph) ?
                setGraph(std::make_shared<BlankNode>(*graph)) :
                setGraph(std::make_shared<IRI>(*graph));
        }
    }

    RDFQuad::RDFQuad(const std::string& subject, const std::string& predicate, const std::string& object, std::string *graph)
            : pimpl_(new RDFQuadImpl(subject, predicate, object))
    {
        if (graph != nullptr && *graph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*graph) ?
                setGraph(std::make_shared<BlankNode>(*graph)) :
                setGraph(std::make_shared<IRI>(*graph));
        }
    }

    std::shared_ptr<Node> RDFQuad::getSubject() const {
        if(pimpl_)
            return pimpl_->getSubject();
        else
            return nullptr;
    }

    std::shared_ptr<Node> RDFQuad::getPredicate() const {
        if(pimpl_)
            return pimpl_->getPredicate();
        else
            return nullptr;
    }

    std::shared_ptr<Node> RDFQuad::getObject() const {
        if(pimpl_)
            return pimpl_->getObject();
        else
            return nullptr;
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

    RDFQuad::~RDFQuad() = default;

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
            graph = std::move(rhs.graph);
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
