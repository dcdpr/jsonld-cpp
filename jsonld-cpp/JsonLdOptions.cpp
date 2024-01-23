#include "jsonld-cpp/JsonLdOptions.h"


// definitions needed here for static members (though not necessary in c++17)
//constexpr const char JsonLdOptions::JSON_LD_1_0[];
//constexpr const char JsonLdOptions::JSON_LD_1_1[];
//constexpr bool JsonLdOptions::DEFAULT_COMPACT_ARRAYS;

bool JsonLdOptions::isFrameExpansion() const {
    return frameExpansion_;
}

void JsonLdOptions::setFrameExpansion(bool frameExpansion) {
    frameExpansion_ = frameExpansion;
}

bool JsonLdOptions::isOrdered() const {
    return ordered_;
}

void JsonLdOptions::setOrdered(bool ordered) {
    ordered_ = ordered;
}

const std::string &JsonLdOptions::getHashAlgorithm() const {
    return hashAlgorithm_;
}

void JsonLdOptions::setHashAlgorithm(const std::string &hashAlgorithm) {
    hashAlgorithm_ = hashAlgorithm;
}
//todo: why are these functions defined here? can they just all be in the header file?
