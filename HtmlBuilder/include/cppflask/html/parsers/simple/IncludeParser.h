#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers {
    class IncludeParser {
    public:
        explicit IncludeParser(std::string& html);
        std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(const HtmlCommand& cmd);

    private:
        std::string& _html;
    };
}
}
