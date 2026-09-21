#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers::setter {
class SetParser {
public:
    inline static const std::string TYPE{"SET"};
    static std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(std::string& html, const HtmlCommand& cmd);
};
}
}
