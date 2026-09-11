#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers::loop {
struct LoopSettings;

class LoopParser {
public:
    explicit LoopParser(const std::string& html);
    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(const HtmlCommand& cmd);

private:
    const std::string& _html;

    std::pair<std::size_t, std::string> findEnd(std::size_t pos);
};
}

}
