#ifndef LIBJSONLD_CPP_RDFTRIPLE_H
#define LIBJSONLD_CPP_RDFTRIPLE_H

#include "jsonld-cpp/RDFNode.h"
#include <memory>
#include <string>


namespace RDF {

    class RDFTriple {
    private:
        std::shared_ptr<Node> subject;
        std::shared_ptr<Node> predicate;
        std::shared_ptr<Node> object;

    protected:
        void setSubject(std::shared_ptr<Node> isubject) { subject = std::move(isubject); }
        void setPredicate(std::shared_ptr<Node> ipredicate) { predicate = std::move(ipredicate); }
        void setObject(std::shared_ptr<Node> iobject) { object = std::move(iobject); }

    public:

        RDFTriple(std::shared_ptr<Node> isubject, std::shared_ptr<Node> ipredicate, std::shared_ptr<Node> iobject);

        RDFTriple(const std::string& isubject, const std::string& ipredicate, const std::string& iobject);

        RDFTriple(const RDFTriple &rhs);
        RDFTriple(RDFTriple&& rhs) noexcept;

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

    struct RDFTriplePtrLess :
            public std::binary_function<const std::shared_ptr<RDFTriple>, const std::shared_ptr<RDFTriple>, bool> {
        bool operator()(const std::shared_ptr<RDFTriple> & lhs, const std::shared_ptr<RDFTriple> & rhs) const {
            return *lhs < *rhs ;
        }
    };

}

#endif //LIBJSONLD_CPP_RDFTRIPLE_H
