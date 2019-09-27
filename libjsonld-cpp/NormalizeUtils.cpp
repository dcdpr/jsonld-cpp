#include "NormalizeUtils.h"
#include "RDFDatasetUtils.h"
#include "sha1.h"
#include "Permutator.h"

using nlohmann::json;


NormalizeUtils::NormalizeUtils(const std::vector<RDF::Quad> &quads,
                               const std::map<std::string, std::map<std::string, std::vector<RDF::Quad>>> &bnodes,
                               const UniqueIdentifierGenerator &uniqueIdentifierGenerator, const JsonLdOptions &opts)
        : quads(quads), bnodes(bnodes), uniqueIdentifierGenerator(uniqueIdentifierGenerator), opts(opts)
{}


// for all unnamed blank node ids, generate unique names for them
std::string NormalizeUtils::hashBlankNodes(const std::vector<std::string> & unnamed_) {
    std::vector<std::string> unnamed = unnamed_;
    std::vector<std::string> nextUnnamed;
    std::map<std::string, std::vector<std::string>> duplicates;
    std::map<std::string, std::string> unique;

        for (int hui = 0;; hui++) {
            if (hui == unnamed.size()) {
                // we are done iterating over unnamed, now name blank nodes
                bool named = false;
                std::vector<std::string> hashes;
                hashes.reserve(unique.size()); // todo: need to make a keySet() function...
                for(auto const & i : unique) {
                    hashes.push_back(i.first);
                }

                std::sort(hashes.begin(), hashes.end());

                for (const auto & hash : hashes) {
                    std::string bnode = unique.at(hash);
                    uniqueIdentifierGenerator.generate(bnode);
                    named = true;
                }

                // continue to hash bnodes if a bnode was assigned a name
                if (named) {
                    // this resets the initial variables, so it seems like it
                    // has to go on the stack
                    // but since this is the end of the function either way, it
                    // might not have to
                    // hashBlankNodes(unnamed);
                    hui = -1;
                    unnamed = nextUnnamed;
                    nextUnnamed.clear();
                    duplicates.clear();
                    unique.clear();
                    continue;
                }
                    // name the duplicate hash bnodes
                else {
                    // names duplicate hash bnodes
                    // enumerate duplicate hash groups in sorted order
                    hashes.clear();
                    hashes.reserve(duplicates.size());
                    for(auto const & i : duplicates) {
                        hashes.push_back(i.first);
                    }

                    std::sort(hashes.begin(), hashes.end());

                    // process each group (iterate over 'hashes', use hash to get group from 'duplicates')
                    for (int pgi = 0;; pgi++) {
                        if (pgi == hashes.size()) {
                            // done, create JSON-LD array
                            std::vector<std::string> normalized;

                            // Note: At this point all bnodes in the set of RDF
                            // quads have been
                            // assigned canonical names, which have been stored
                            // in the 'uniqueIdentifierGenerator' object.
                            // Here each quad is updated by assigning each of
                            // its bnodes its new name
                            // via the 'uniqueIdentifierGenerator' object

                            // update bnode names in each quad and serialize
                            for (int cai = 0; cai < quads.size(); ++cai) {
                                RDF::Quad quad = quads.at(cai);

                                // todo: replace this with an iteration over function pointers
                                auto n = quad.getSubject();
                                if(n != nullptr) {
                                    if(n->isBlankNode()) {
                                        std::string id = n->getValue();
                                        if(id.find("_:c14n") != 0){
                                            n->setValue(uniqueIdentifierGenerator.generate(id));
                                        }
                                    }
                                }
                                n = quad.getObject();
                                if(n != nullptr) {
                                    if(n->isBlankNode()) {
                                        std::string id = n->getValue();
                                        if(id.find("_:c14n") != 0){
                                            n->setValue(uniqueIdentifierGenerator.generate(id));
                                        }
                                    }
                                }
                                n = quad.getGraph();
                                if(n != nullptr) {
                                    if(n->isBlankNode()) {
                                        std::string id = n->getValue();
                                        if(id.find("_:c14n") != 0){
                                            n->setValue(uniqueIdentifierGenerator.generate(id));
                                        }
                                    }
                                }

                                n = quad.getGraph();
                                if(n != nullptr) {
                                    std::string name;
                                    name = n->getValue();
                                    normalized.push_back(RDFDatasetUtils::toNQuad(quad, &name));
                                }
                                else {
                                    normalized.push_back(RDFDatasetUtils::toNQuad(quad, nullptr));
                                }
                            }

                            // sort normalized output
                            std::sort(normalized.begin(), normalized.end());

                            // handle output format
//                            if (opts.format != null) {
//                                if (JsonLdConsts.APPLICATION_NQUADS.equals(options.format)) {
                                    std::stringstream rval;
                                    for (const auto & n : normalized) {
                                        rval << n;
                                    }
                                    return rval.str();
//                                } else {
//                                    throw new JsonLdError(JsonLdError.Error.UNKNOWN_FORMAT,
//                                                          options.format);
//                                }
//                            }
//                            final StringBuilder rval = new StringBuilder();
//                            for (final String n : normalized) {
//                                rval.append(n);
//                            }
//                            return parseNQuads(rval.toString());
                        }

                        // name each group member
                        std::vector<std::string> group = duplicates.at(hashes.at(pgi));
                        std::vector<HashResult> results;
                        for (int n = 0;; n++) {
                            if (n == group.size()) {
                                // name bnodes in hash order

                                std::sort(results.begin(), results.end(),
                                        [](const HashResult & result1, const HashResult & result2) {
                                    return result1.hash < result2.hash;
                                });
                                for (auto r : results) {
                                    // name all bnodes in path namer in key-entry order
                                    for (auto key : r.pathNamer.getIdentifiersInsertionOrder()) {
                                        auto s = uniqueIdentifierGenerator.generate(key);
                                    }
                                }
                                break;
                            } else {
                                // skip already-named bnodes
                                std::string bnode = group.at(n);
                                if (uniqueIdentifierGenerator.exists(bnode)) {
                                    continue;
                                }

                                // hash bnode paths
                                UniqueIdentifierGenerator pathNamer;
                                pathNamer.generate(bnode);
                                HashResult result = hashPaths(bnode, pathNamer);
                                results.push_back(result);
                            }
                        }
                    }
                }
            }

            // hash unnamed bnode
            std::string bnode = unnamed.at(hui);
            std::string hash = hashQuads(bnode);

            // store hash as unique or a duplicate
            if (duplicates.count(hash)) {
                duplicates.at(hash).push_back(bnode);
                nextUnnamed.push_back(bnode);
            } else if (unique.count(hash)) {
                std::vector<std::string> tmp;
                tmp.push_back(unique.at(hash));
                tmp.push_back(bnode);
                duplicates[hash] = tmp;
                nextUnnamed.push_back(unique.at(hash));
                nextUnnamed.push_back(bnode);
                unique.erase(hash);
            } else {
                unique[hash] = bnode;
            }
        }
}

NormalizeUtils::HashResult NormalizeUtils::hashPaths(std::string id, UniqueIdentifierGenerator pathNameGenerator) {

    std::map<std::string, std::vector<std::string>> groups;
    std::vector<RDF::Quad> quads = bnodes.at(id).at("quads");
    SHA1 md;

    for (int hpi = 0;; hpi++) {
        if (hpi == quads.size()) {
            // done , hash groups
            std::vector<std::string> groupHashes;
            groupHashes.reserve(groups.size()); // todo: need to make a keySet() function...
            for (auto const &i : groups) {
                groupHashes.push_back(i.first);
            }

            std::sort(groupHashes.begin(), groupHashes.end());

            for (int hgi = 0;; hgi++) {
                if (hgi == groupHashes.size()) {
                    HashResult res;
                    res.hash = md.digest();
                    res.pathNamer = pathNameGenerator;
                    return res;
                }
                // digest group hash
                std::string groupHash = groupHashes.at(hgi);
                md.update(groupHash);

                // choose a path and namer from the permutations
                std::shared_ptr<std::string> chosenPath = nullptr;
                UniqueIdentifierGenerator chosenNamer;
                Permutator permutator(groups.at(groupHash));

//                std::vector<std::string> permutation_of_blankNodes = groups.at(groupHash); // todo: rename blankNodes?
//                std::sort(permutation_of_blankNodes.begin(), permutation_of_blankNodes.end());

                while (true) {
                    bool contPermutation = false;
                    bool breakOut = false;
                    std::vector<std::string> permutation = permutator.next();
                    UniqueIdentifierGenerator pathNameGeneratorCopy = pathNameGenerator;

                    // build adjacent path
                    std::string path = "";
                    std::vector<std::string> recurse;
                    for (auto bnode : permutation) {
                        // use canonical name if available
                        if (uniqueIdentifierGenerator.exists(bnode)) {
                            path += uniqueIdentifierGenerator.generate(bnode);
                        } else {
                            // recurse if bnode isn't named in the path yet
                            if (!pathNameGeneratorCopy.exists(bnode)) {
                                recurse.push_back(bnode);
                            }
                            path += pathNameGeneratorCopy.generate(bnode);
                        }

                        // skip permutation if path is already >= chosen path
                        if (chosenPath != nullptr && path.length() >= chosenPath->length() && path > *chosenPath) {
//                            if(std::next_permutation(permutation_of_blankNodes.begin(), permutation_of_blankNodes.end())) {
                            if(permutator.hasNext()) {
                                contPermutation = true;
                            } else {
                                // digest chosen path and update namer
                                md.update(*chosenPath);
                                pathNameGenerator = chosenNamer;
                                // hash the nextGroup
                                breakOut = true;
                            }
                            break;
                        }
                    }

                    // if we should do the next permutation
                    if (contPermutation) {
                        continue;
                    }
                    // if we should stop processing this group
                    if (breakOut) {
                        break;
                    }

                    // does the next recursion
                    for (int nrn = 0;; nrn++) {
                        if (nrn == recurse.size()) {
                            if (chosenPath == nullptr || path < *chosenPath) {
                                chosenPath = std::make_shared<std::string>(path);
                                chosenNamer = pathNameGeneratorCopy;
                            }
                            if (!permutator.hasNext()) {
                                // digest chosen path and update namer
                                md.update(*chosenPath);
                                pathNameGenerator = chosenNamer;
                                // hash the nextGroup
                                breakOut = true;
                            }
                            break;
                        }

                        // do recursion
                        std::string bnode = recurse.at(nrn);
                        HashResult result = hashPaths(bnode, pathNameGeneratorCopy);
                        path += pathNameGeneratorCopy.generate(bnode) + "<" + result.hash + ">";
                        pathNameGeneratorCopy = result.pathNamer;

                        // skip permutation if path is already >= chosen path
                        if (chosenPath != nullptr && path.length() >= chosenPath->length() && path > *chosenPath) {
                            if (!permutator.hasNext()) {
                                // digest chosen path and update namer
                                md.update(*chosenPath);
                                pathNameGenerator = chosenNamer;
                                // hash the nextGroup
                                breakOut = true;
                            }
                            break;
                        }
                        // do next recursion
                    }

                    // if we should stop processing this group
                    if (breakOut) {
                        break;
                    }
                }
            }
        }
        // get adjacent bnode
        RDF::Quad quad = quads.at(hpi);
        std::shared_ptr<std::string> bnode = getAdjacentBlankNodeName(quad.getSubject(), id);
        std::string direction;
        if (bnode != nullptr) {
            // normal property
            direction = "p";
        } else {
            bnode = getAdjacentBlankNodeName(quad.getObject(), id);
            if (bnode != nullptr) {
                // reverse property
                direction = "r";
            }
        }

        if (bnode != nullptr) {
            // get bnode name (try canonical, path, then hash)
            std::string name;
            if (uniqueIdentifierGenerator.exists(*bnode)) {
                name = uniqueIdentifierGenerator.generate(*bnode);
            } else if (pathNameGenerator.exists(*bnode)) {
                name = pathNameGenerator.generate(*bnode);
            } else {
                name = hashQuads(*bnode);
            }

            // hash direction, property, end bnode name/hash
            SHA1 md1;
            md1.update(direction);
            md1.update(quad.getPredicate()->getValue());
            md1.update(name);
            std::string groupHash = md1.digest();
            if (groups.count(groupHash)) {
                groups[groupHash].push_back(*bnode);
            } else {
                std::vector<std::string> tmp;
                tmp.push_back(*bnode);
                groups[groupHash] = tmp;
            }
        }
    }
}

std::string NormalizeUtils::hashQuads(std::string id) {
    // return cached hash
    if(cachedHashes.count(id))
        return cachedHashes[id];

    // serialize all of bnode's quads
    std::vector<RDF::Quad> quads = bnodes.at(id).at("quads");
    std::vector<std::string> nquads;
    for (auto & quad : quads) {
        std::string graphName;
        auto name = quad.getGraph();
        if(name != nullptr)
            graphName = name->getValue();
        nquads.push_back(
                RDFDatasetUtils::toNQuad(quad,name == nullptr ? nullptr : &graphName, &id));
    }
    // sort serialized quads
    std::sort(nquads.begin(), nquads.end());
    // return hashed quads
    std::string hash = sha1(nquads);
    cachedHashes[id] = hash;
    return hash;
}


/**
 * A helper function that gets the blank node name from an RDF quad node
 * (subject or object). If the node is a blank node and its value does not
 * match the given blank node ID, it will be returned.
 *
 * @param node
 *            the RDF quad node.
 * @param id
 *            the ID of the blank node to look next to.
 *
 * @return the adjacent blank node name or null if none was found.
 */
std::shared_ptr<std::string> NormalizeUtils::getAdjacentBlankNodeName(std::shared_ptr<RDF::Node> node, std::string id) {

    if (node->isBlankNode()) {
        std::string v = node->getValue();
        if (v != id)
            return std::make_shared<std::string>(v);
        else
            return nullptr;
    }
    else
        return nullptr;
}


