#ifndef LIBJSONLD_CPP_RDFTRIPLE_H
#define LIBJSONLD_CPP_RDFTRIPLE_H

#include <memory>
#include <string>
#include <utility>

#include "jsonld-cpp/RDFNode.h"


namespace RDF {

    class RDFTriple {
    private:
        std::shared_ptr<Node> subject;
        std::shared_ptr<Node> predicate;
        std::shared_ptr<Node> object;

    protected:
        void setSubject(std::shared_ptr<Node> subject) { this->subject = std::move(subject); }
        void setPredicate(std::shared_ptr<Node> predicate) { this->predicate = std::move(predicate); }
        void setObject(std::shared_ptr<Node> object) { this->object = std::move(object); }

    public:

        RDFTriple(std::shared_ptr<Node> subject, std::shared_ptr<Node> predicate, std::shared_ptr<Node> object);

        RDFTriple(const std::string& subject, const std::string& predicate, const std::string& object);

        RDFTriple(const RDFTriple &rhs);
        RDFTriple(RDFTriple&& rhs) noexcept;

        virtual ~RDFTriple();

        RDFTriple& operator= (const RDFTriple &rhs);
        RDFTriple& operator= (RDFTriple&& rhs) noexcept;

        virtual std::shared_ptr<Node> getPredicate() const;
        virtual std::shared_ptr<Node> getObject() const;
        virtual std::shared_ptr<Node> getSubject() const;

        std::string toString() const;

        friend bool operator==(const RDFTriple &lhs, const RDFTriple &rhs);

    };

    bool operator==(const RDFTriple &lhs, const RDFTriple &rhs);
    bool operator!=(const RDFTriple &lhs, const RDFTriple &rhs);
    bool operator<(const RDFTriple &lhs, const RDFTriple &rhs);
    bool operator>(const RDFTriple &lhs, const RDFTriple &rhs);
    bool operator<=(const RDFTriple &lhs, const RDFTriple &rhs);
    bool operator>=(const RDFTriple &lhs, const RDFTriple &rhs);

}

#endif //LIBJSONLD_CPP_RDFTRIPLE_H
