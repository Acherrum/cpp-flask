#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <functional>

namespace cppflask::html::nodes {

    class PipedVariableNode : public BaseNode {
    public:
        PipedVariableNode(const std::string& var, const std::string& pipeMethod);

        ~PipedVariableNode() override;
        std::string render(JsonObject& data) override;

    private:
        std::function<std::string(JsonObject&)> _pipeMethod;
    };

}