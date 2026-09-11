#pragma once

#include <string>

namespace cppflask::html {
struct HtmlCommand {
    std::string cmd;
    std::size_t startPos;
    std::size_t endPos;
};
}
