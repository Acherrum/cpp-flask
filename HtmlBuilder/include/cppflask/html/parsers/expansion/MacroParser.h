#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers::expansion {
    class MacroParser {
    public:
        inline static const std::string TYPE{"MACRO"};
        static std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(std::string& html, const HtmlCommand& cmd);
    };
}
}
