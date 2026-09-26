#pragma once

#include <string>
#include <vector>

namespace cppflask::html::parsers {
struct MacroDefinition {
    std::string name;
    std::vector<std::string> arguments;
    std::string body;
};
}