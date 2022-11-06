#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/NQuadsSerialization.h"

using nlohmann::json;

namespace RDF {

    bool operator!=(const RDF::RDFQuad &lhs, const RDF::RDFQuad &rhs) {
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

    RDFQuad::RDFQuad(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject,
                     std::string *igraph) {
        setSubject(std::move(isubject));
        setPredicate(std::move(ipredicate));
        setObject(std::move(iobject));

        if (igraph != nullptr && *igraph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*igraph) ?
            setGraph(std::make_shared<BlankNode>(*igraph)) :
            setGraph(std::make_shared<IRI>(*igraph));
        }
    }

    RDFQuad::RDFQuad(const std::string& isubject, const std::string& ipredicate, const std::string& iobject, std::string *igraph) {
        BlankNodeNames::hasFormOfBlankNodeName(isubject) ?
            setSubject(std::make_shared<BlankNode>(isubject)) :
            setSubject(std::make_shared<IRI>(isubject));

        setPredicate(std::make_shared<IRI>(ipredicate));

        BlankNodeNames::hasFormOfBlankNodeName(iobject) ?
            setObject(std::make_shared<BlankNode>(iobject)) :
            setObject(std::make_shared<IRI>(iobject)); // todo: literal?

        if (igraph != nullptr && *igraph != "@default") {
            BlankNodeNames::hasFormOfBlankNodeName(*igraph) ?
                setGraph(std::make_shared<BlankNode>(*igraph)) :
                setGraph(std::make_shared<IRI>(*igraph));
        }
    }

    std::shared_ptr<Node> RDFQuad::getSubject() const {
        return subject;
    }

    std::shared_ptr<Node> RDFQuad::getPredicate() const {
        return predicate;
    }

    std::shared_ptr<Node> RDFQuad::getObject() const {
        return object;
    }

    std::shared_ptr<Node> RDFQuad::getGraph() const {
        return graph;
    }

    std::string RDFQuad::toString() const {
        return NQuadsSerialization::toNQuad(*this);
    }

    RDFQuad::RDFQuad(const RDFQuad &rhs) {
        if(rhs.subject->isBlankNode())
            subject = std::make_shared<BlankNode>(rhs.subject->getValue());
        else if(rhs.subject->isIRI())
            subject = std::make_shared<IRI>(rhs.subject->getValue());

        predicate = std::make_shared<IRI>(rhs.predicate->getValue());

        if(rhs.object->isBlankNode())
            object = std::make_shared<BlankNode>(rhs.object->getValue());
        else if(rhs.object->isIRI())
            object = std::make_shared<IRI>(rhs.object->getValue());
        else if (rhs.object->isLiteral()) {
            std::string dataType = rhs.object->getDatatype();
            std::string language = rhs.object->getLanguage();
            object = std::make_shared<Literal>(rhs.object->getValue(), &dataType, &language);
        }

        if(rhs.graph && rhs.graph->isBlankNode())
            graph = std::make_shared<BlankNode>(rhs.graph->getValue());
        else if(rhs.graph && rhs.graph->isIRI())
            graph = std::make_shared<IRI>(rhs.graph->getValue());
    }

    RDFQuad::RDFQuad(RDFQuad&& rhs) noexcept {
        subject = std::move(rhs.subject);
        predicate = std::move(rhs.predicate);
        object = std::move(rhs.object);
        graph = std::move(rhs.graph);
    }

    RDFQuad & RDFQuad::operator=(const RDFQuad &rhs) {
        if (&rhs != this) {
            subject = std::shared_ptr<Node>{rhs.subject};
            predicate = std::shared_ptr<Node>{rhs.predicate};
            object = std::shared_ptr<Node>{rhs.object};
            graph = std::shared_ptr<Node>{rhs.graph};
        }
        return *this;
    }

    RDFQuad& RDFQuad::operator= (RDFQuad&& rhs) noexcept {
        if (&rhs != this) {
            subject = std::shared_ptr<Node>{rhs.subject};
            predicate = std::shared_ptr<Node>{rhs.predicate};
            object = std::shared_ptr<Node>{rhs.object};
            graph = std::shared_ptr<Node>{rhs.graph};
            rhs.subject = nullptr;
            rhs.predicate = nullptr;
            rhs.object = nullptr;
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

}
