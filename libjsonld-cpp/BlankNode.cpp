#include "BlankNode.h"
#include "JsonLdConsts.h"

bool BlankNode::isBlankNodeName(const std::string &name) {
    return name.find(JsonLdConsts::BLANK_NODE_PREFIX) == 0;
}
