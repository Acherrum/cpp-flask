#include "cppflask/html/nodes/HtmlNode.h"

#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {
HtmlNode::HtmlNode(const std::string& html) : BaseNode{"Html", html} {}
HtmlNode::~HtmlNode() = default;

std::string HtmlNode::render(JsonObject&)
{
    return _html;
}

}