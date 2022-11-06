#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "jsonld-cpp/BlankNodeNames.h"
#include "jsonld-cpp/NQuadsSerialization.h"

using nlohmann::json;

namespace RDF {

    bool operator!=(const RDF::RDFTriple &lhs, const RDF::RDFTriple &rhs) {
        return !(rhs == lhs);
    }

    bool operator<(const RDFTriple &lhs, const RDFTriple &rhs) {
        NodePtrLess less;
        NodePtrEquals equals;
        if(!equals(lhs.getSubject(), rhs.getSubject()))
            return less(lhs.getSubject(), rhs.getSubject());
        if(!equals(lhs.getPredicate(), rhs.getPredicate()))
            return less(lhs.getPredicate(), rhs.getPredicate());

        return less(lhs.getObject(), rhs.getObject());
    }

    bool operator>(const RDFTriple &lhs, const RDFTriple &rhs) {
        return rhs < lhs;
    }

    bool operator<=(const RDFTriple &lhs, const RDFTriple &rhs) {
        return !(rhs < lhs);
    }

    bool operator>=(const RDFTriple &lhs, const RDFTriple &rhs) {
        return !(lhs < rhs);
    }

    RDFTriple::RDFTriple(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject) {
        setSubject(std::move(isubject));
        setPredicate(std::move(ipredicate));
        setObject(std::move(iobject));
    }

    RDFTriple::RDFTriple(const std::string& isubject, const std::string& ipredicate, const std::string& iobject) {
        BlankNodeNames::hasFormOfBlankNodeName(isubject) ?
            setSubject(std::make_shared<BlankNode>(isubject)) :
            setSubject(std::make_shared<IRI>(isubject));

        setPredicate(std::make_shared<IRI>(ipredicate));

        BlankNodeNames::hasFormOfBlankNodeName(iobject) ?
            setObject(std::make_shared<BlankNode>(iobject)) :
            setObject(std::make_shared<IRI>(iobject)); // todo: literal?
    }

    std::shared_ptr<Node> RDFTriple::getPredicate() const {
        return predicate;
    }

    std::shared_ptr<Node> RDFTriple::getObject() const {
        return object;
    }

    std::shared_ptr<Node> RDFTriple::getSubject() const {
        return subject;
    }

    std::string RDFTriple::toString() const {
        return NQuadsSerialization::toNQuad(*this);
    }

    RDFTriple::RDFTriple(const RDFTriple &rhs) {
        if (rhs.subject->isBlankNode())
            subject = std::make_shared<BlankNode>(rhs.subject->getValue());
        else if (rhs.subject->isIRI())
            subject = std::make_shared<IRI>(rhs.subject->getValue());

        predicate = std::make_shared<IRI>(rhs.predicate->getValue());

        if (rhs.object->isBlankNode())
            object = std::make_shared<BlankNode>(rhs.object->getValue());
        else if (rhs.object->isIRI())
            object = std::make_shared<IRI>(rhs.object->getValue());
        else if (rhs.object->isLiteral()) {
            std::string dataType = rhs.object->getDatatype();
            std::string language = rhs.object->getLanguage();
            object = std::make_shared<Literal>(rhs.object->getValue(), &dataType, &language);
        }
    }

    RDFTriple::RDFTriple(RDFTriple&& rhs) noexcept {
        subject = std::move(rhs.subject);
        predicate = std::move(rhs.predicate);
        object = std::move(rhs.object);
    }

    RDFTriple &RDFTriple::operator=(const RDFTriple &rhs) {
        if (&rhs != this) {
            subject = std::shared_ptr<Node>{rhs.subject};
            predicate = std::shared_ptr<Node>{rhs.predicate};
            object = std::shared_ptr<Node>{rhs.object};
        }
        return *this;
    }

    RDFTriple& RDFTriple::operator= (RDFTriple&& rhs) noexcept {
        if (&rhs != this) {
            subject = std::shared_ptr<Node>{rhs.subject};
            predicate = std::shared_ptr<Node>{rhs.predicate};
            object = std::shared_ptr<Node>{rhs.object};
            rhs.subject = nullptr;
            rhs.predicate = nullptr;
            rhs.object = nullptr;
        }
        return *this;
    }

    bool operator==(const RDF::RDFTriple &lhs, const RDF::RDFTriple &rhs) {
        NodePtrEquals equals;
        return
                equals(lhs.getSubject(), rhs.getSubject()) &&
                equals(lhs.getPredicate(), rhs.getPredicate()) &&
                equals(lhs.getObject(), rhs.getObject());
    }

}
