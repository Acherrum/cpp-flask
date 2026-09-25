#include "cppflask/html/parsers/combine/BlocksParser.h"

#include "cppflask/html/HtmlCommand.h"

namespace {
const std::string END_BLOCK_CMD{"{% end_block %}"};
}

namespace cppflask::html::parsers {
    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> BlocksParser::parse(std::string& _html, const HtmlCommand& cmd) {

        auto endCommandPos = _html.find(END_BLOCK_CMD, cmd.endPos);
        if (endCommandPos == std::string::npos) {
            _html.replace(cmd.startPos, cmd.endPos - cmd.startPos, "Parse error: unused blocks command has no end command: " + cmd.cmd);
            return {cmd.startPos, nullptr};
        }

        _html.erase(cmd.startPos, endCommandPos + END_BLOCK_CMD.length());

        return {cmd.startPos, nullptr};
    }
}
