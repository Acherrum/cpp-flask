#pragma once

#include <string>

namespace cppflask::html::parsers::loop {
struct LoopSettings {
    std::string start;
    std::string end;
    std::string variable;
    std::string dataVar;
};
}