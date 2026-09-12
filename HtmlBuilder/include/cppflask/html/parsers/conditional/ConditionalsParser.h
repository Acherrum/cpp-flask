#pragma once

#include <string>
#include <memory>

#include "cppflask/html/nodes/BaseNode.h"

namespace cppflask::html {
struct HtmlCommand;

namespace parsers::conditional {
class ConditionalsParser {
public:
    explicit ConditionalsParser(const std::string& html);
    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> parse(const HtmlCommand& cmd);

private:
    const std::string& _html;

    [[nodiscard]] std::pair<std::size_t, std::string> findEnd(std::size_t pos, const std::string& endText = "{% END_IF %}") const;
};
}

}
