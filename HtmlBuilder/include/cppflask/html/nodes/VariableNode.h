#pragma once

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html::nodes {

    class VariableNode : public BaseNode {
    public:
        explicit VariableNode(const std::string& html);

        ~VariableNode() override;
        std::string render(JsonObject& data) const override;
    };

}