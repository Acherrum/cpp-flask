#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <functional>

namespace cppflask::html::nodes {

    class PipedVariableNode : public BaseNode {
    public:
        PipedVariableNode(const std::string& var, const std::string& pipeExpression);

        ~PipedVariableNode() override;
        std::string render(JsonObject& data) const override;

    private:
        std::vector<std::string> _pipeMethods;
    };

}