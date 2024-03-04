#include <utility>

#include "jsonld-cpp/MediaType.h"

MediaType::MediaType(std::string type, std::string subType) :
        type(std::move(type)), subType(std::move(subType)) {}

MediaType MediaType::of(const std::string &type, const std::string &subType) {
    return {type, subType};
}

bool MediaType::operator==(const MediaType &rhs) const {
    return type == rhs.type &&
           subType == rhs.subType;
}

bool MediaType::operator!=(const MediaType &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const MediaType &type) {
    os << type.type << "/" << type.subType;
    return os;
}
