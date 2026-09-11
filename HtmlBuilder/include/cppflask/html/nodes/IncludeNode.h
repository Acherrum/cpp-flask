#pragma once

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask {

    class JsonObject;

    namespace html::nodes {

    class IncludeNode : public BaseNode {
    public:
        explicit IncludeNode(const std::string& variable);

        ~IncludeNode() override;

        std::string render(JsonObject& data) override;

    private:
        std::string _variable;
    };

    }
}
