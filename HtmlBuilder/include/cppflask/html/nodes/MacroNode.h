#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <vector>

namespace cppflask {
    class JsonObject;

    namespace html::nodes {
        class MacroNode : public BaseNode {
        public:
            MacroNode(std::string body, std::vector<std::string> arguments, std::vector<std::string> values);

            ~MacroNode() override;

            std::string render(JsonObject& data) const override;

        private:
            std::string _body;
            std::vector<std::string> _arguments;
            std::vector<std::string> _values;
        };
    }
}
