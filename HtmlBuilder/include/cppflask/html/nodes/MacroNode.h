#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace cppflask {
    class JsonObject;

    namespace html::nodes {
        class MacroNode : public BaseNode {
        public:
            MacroNode(std::string body, const std::vector<std::string>& arguments, const std::vector<std::string>& values);

            ~MacroNode() override;

            std::string render(JsonObject& data) const override;

        private:
            const std::string _body;
            std::unordered_map<std::string, std::string> _arguments;
        };
    }
}
