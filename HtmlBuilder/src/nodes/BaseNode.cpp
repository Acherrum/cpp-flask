#include "cppflask/html/nodes/BaseNode.h"


namespace cppflask::html::nodes {
BaseNode::BaseNode(const std::string& type, const std::string& html) : _type{type}, _html{html} {}

BaseNode::~BaseNode() = default;

const std::string& BaseNode::getType() const { return _type; }

}