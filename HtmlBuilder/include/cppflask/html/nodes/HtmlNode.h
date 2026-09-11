#pragma once

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html::nodes {

class HtmlNode : public BaseNode {
public:
    HtmlNode(const std::string& html);
    
    ~HtmlNode() override;
    std::string render(JsonObject& data) override;
};

}