#include "jsonld-cpp/RDFDatasetComparison.h"
#include "jsonld-cpp/RDFDataset.h"
#include "jsonld-cpp/RDFTriple.h"
#include "jsonld-cpp/RDFQuad.h"
#include "jsonld-cpp/JsonLdOptions.h"
#include "jsonld-cpp/JsonLdUtils.h"
#include "jsonld-cpp/DoubleFormatter.h"
#include "JsonLdError.h"
#include "RDFDatasetUtils.h"

using nlohmann::json;

namespace RDF {

    bool compareGraphName(const std::shared_ptr<Node>& name1, const std::shared_ptr<Node>& name2, std::map<std::string, std::string> *mapping) {

        if (!name1 && !name2) {
            return true;
        }

        if (!name1 || !name2) {
            return false;
        }

        if (name1->isBlankNode() && name2->isBlankNode()) {
            return name1->getValue() ==
                    (mapping != nullptr ? mapping->at(name2->getValue()) : name2->getValue());

        } else if (name1->isIRI() && name2->isIRI()) {
            return name1->getValue() == name2->getValue();
        }
        return false;
    }

    bool compareObject(const std::shared_ptr<Node>& object1, const std::shared_ptr<Node>& object2, std::map<std::string, std::string> *mapping) {
        if(object1->isBlankNode() && object2->isBlankNode()) {
            return object1->getValue() ==
                   (mapping != nullptr ? mapping->at(object2->getValue()) : object2->getValue());
        }
        else if((object1->isIRI() && object2->isIRI()) || (object1->isLiteral() && object2->isLiteral())) {
            return object1->getValue() == object2->getValue();
        }
        return false;
    }

    bool comparePredicate(const std::shared_ptr<Node>& predicate1, const std::shared_ptr<Node>& predicate2) {
        return predicate1->getValue() == predicate2->getValue();
    }

    bool compareSubject(const std::shared_ptr<Node>& subject1, const std::shared_ptr<Node>& subject2, std::map<std::string, std::string> *mapping) {
        if(subject1->isBlankNode() && subject2->isBlankNode()) {
            return subject1->getValue() ==
                   (mapping != nullptr ? mapping->at(subject2->getValue()) : subject2->getValue());
        }
        else if(subject1->isIRI() && subject2->isIRI()) {
            return subject1->getValue() == subject2->getValue();
        }
        return false;
    }

    bool compareNQuad(RDFQuad &nquad1, RDFQuad &nquad2, std::map<std::string, std::string> *mapping) {
        return
                compareSubject(nquad1.getSubject(), nquad2.getSubject(), mapping) &&
                comparePredicate(nquad1.getPredicate(), nquad2.getPredicate()) &&
                compareObject(nquad1.getObject(), nquad2.getObject(), mapping) &&
                compareGraphName(nquad1.getGraph(), nquad2.getGraph(), mapping);
    }

    bool
    compareNQuads(
            const std::vector<RDFQuad>& nquads1,
            const std::vector<RDFQuad>& nquads2,
            std::map<std::string, std::string> *mapping) {

        std::vector<RDFQuad> remaining(nquads2);

        for (RDFQuad nquad1 : nquads1) {

            bool found = false;

            for (auto nquad2_it = remaining.begin(); nquad2_it != remaining.end(); ) {
                found = compareNQuad(*nquad2_it, nquad1, mapping);

                if (found) {
                    nquad2_it = remaining.erase(nquad2_it);
                    break;
                } else {
                    ++nquad2_it;
                }
            }

            if (!found) {
                return false;
            }
        }
        return remaining.empty();
    }

    bool areIsomorphic(const RDFDataset & dataset1, const RDFDataset & dataset2) {

        // if datasets are empty
        if (dataset1.empty() && dataset2.empty()) {
            return true;
        }

        // compare total number of n-quads
        if (dataset1.numTriples() != dataset2.numTriples()) {
            return false;
        }

        // convert dataset's "graphnames + triples" to quads for easier processing
        std::vector<RDFQuad> nquads1 = dataset1.getAllGraphsAsQuads();
        std::vector<RDFQuad> nquads2 = dataset2.getAllGraphsAsQuads();

        std::vector<RDFQuad> nquads1_noBlanks;
        std::copy_if(nquads1.begin(), nquads1.end(), std::back_inserter(nquads1_noBlanks),
                     [](const RDFQuad& q) {
                         return !(q.getObject()->isBlankNode() ||
                                  q.getSubject()->isBlankNode() ||
                                  (q.getGraph() && q.getGraph()->isBlankNode()));
                     });

        std::vector<RDFQuad> nquads2_noBlanks;
        std::copy_if(nquads2.begin(), nquads2.end(), std::back_inserter(nquads2_noBlanks),
                     [](const RDFQuad& q) {
                         return !(q.getObject()->isBlankNode() ||
                                  q.getSubject()->isBlankNode() ||
                                  (q.getGraph() && q.getGraph()->isBlankNode()));
                     });

        // compare number of non-blank node n-quads
        if (nquads1_noBlanks.size() != nquads2_noBlanks.size()) {
            return false;
        }

        if (!compareNQuads(nquads1_noBlanks, nquads2_noBlanks, nullptr)) {
            return false;
        }

        // if datasets have no blank nodes
        if (nquads1_noBlanks.size() == dataset2.numTriples()) {
            return true;
        }

        std::vector<RDFQuad> nquads1_blanks;
        std::copy_if(nquads1.begin(), nquads1.end(), std::back_inserter(nquads1_blanks),
                     [](const RDFQuad& q) {
                         return q.getObject()->isBlankNode() ||
                                q.getSubject()->isBlankNode() ||
                                (q.getGraph() && q.getGraph()->isBlankNode());
                     });

        std::vector<RDFQuad> nquads2_blanks;
        std::copy_if(nquads2.begin(), nquads2.end(), std::back_inserter(nquads2_blanks),
                     [](const RDFQuad& q) {
                         return q.getObject()->isBlankNode() ||
                                q.getSubject()->isBlankNode() ||
                                (q.getGraph() && q.getGraph()->isBlankNode());
                     });

        // compare number of blank node n-quads
        if (nquads1_blanks.size() != nquads2_blanks.size()) {
            return false;
        }

        // create mappings from nquads2_blanks to nquads1_blanks
        NodeMapper mapper = NodeMapper::create(nquads2_blanks, nquads1_blanks);

        int iteration = 0;

        while (mapper.hasNext()) {

            std::map<std::string, std::string> mapping = mapper.next();
            if (compareNQuads(nquads1_blanks, nquads2_blanks, &mapping)) {
                return true;
            }

            iteration++;

            if (iteration >=  5000000) {
                std::cerr << "Too many permutations [" << mapper.permutations() << "]" << std::endl;
                return false;
            }
        }

        return false;
    }


}
