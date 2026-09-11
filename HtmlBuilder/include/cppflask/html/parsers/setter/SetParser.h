#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask {
    class JsonObject;

    namespace html {
        struct HtmlCommand;

        namespace parsers::setter {
            class SetParser {
            public:
                explicit SetParser(const std::string& html);
                std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(const HtmlCommand& cmd);

            private:
                const std::string& _html;
            };
        }
    }
}
