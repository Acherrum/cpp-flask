#include "cppflask/html/parsers/simple/IncludeParser.h"

#include <fstream>
#include <sstream>

#include "cppflask/html/nodes/IncludeNode.h"
#include "cppflask/html/HtmlCommand.h"

namespace {
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "Parse error: could not open file " + filename;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        auto contents = buffer.str();
        file.close();
        return contents;
    }
}

namespace cppflask::html::parsers {

    IncludeParser::IncludeParser(std::string& html) : _html{html} {}

    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> IncludeParser::parse(const HtmlCommand& cmd) {

        auto includeContentPos = cmd.cmd.find('(');
        auto includeContentEndPos = cmd.cmd.rfind(')');
        if (includeContentPos == std::string::npos || includeContentEndPos == std::string::npos || includeContentPos > includeContentEndPos) {
            _html.replace(cmd.startPos, cmd.endPos - cmd.startPos, "Parse error: include command not formatted correctly.");
            return {cmd.startPos, nullptr};
        }

        ++includeContentPos;
        auto includeContent = cmd.cmd.substr(includeContentPos, includeContentEndPos - includeContentPos);

        if (!includeContent.empty() && includeContent.at(0) == '$') {
            return {cmd.endPos, std::make_unique<nodes::IncludeNode>(includeContent.substr(1))};
        }

        auto html = readFile(includeContent);
        _html.replace(cmd.startPos, cmd.endPos - cmd.startPos, html);
        return {cmd.startPos, nullptr};
    }

}
