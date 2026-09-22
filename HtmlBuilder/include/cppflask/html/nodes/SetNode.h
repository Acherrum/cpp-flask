#pragma once

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask {

    class JsonObject;

    namespace html::nodes {

        class SetNode : public BaseNode {
        public:
            SetNode(std::string key, std::string expression);

            ~SetNode() override;

            std::string render(JsonObject& data) const override;

        private:
            std::string _key;
            std::string _expression;
        };

    }

}
