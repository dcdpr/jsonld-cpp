#include "jsonld-cpp/MediaType.h"

MediaType::MediaType(const std::string &itype, const std::string &isubType) :
        type(itype), subType(isubType) {}

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
