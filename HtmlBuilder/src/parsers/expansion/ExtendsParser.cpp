#include "cppflask/html/parsers/expansion/ExtendsParser.h"

#include <fstream>
#include <sstream>

#include "cppflask/html/nodes/ExtendsNode.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/parsers/expansion/BlocksEvaluator.h"

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

namespace cppflask::html::parsers::expansion {
    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> ExtendsParser::parse(std::string& _html, const HtmlCommand& cmd) {

        auto baseFilePos = cmd.cmd.find('(');
        auto baseFileEndPos = cmd.cmd.rfind(')');
        if (baseFilePos == std::string::npos || baseFileEndPos == std::string::npos || baseFilePos > baseFileEndPos) {
            _html.replace(cmd.startPos, cmd.endPos - cmd.startPos, "Parse error: extends command not formatted correctly.");
            return {cmd.startPos, nullptr};
        }

        ++baseFilePos;
        auto baseFile = cmd.cmd.substr(baseFilePos, baseFileEndPos - baseFilePos);

        if (baseFile.empty()) {
            return {cmd.endPos, std::make_unique<nodes::HtmlNode>("Parse error: extends command filename is empty.")};
        }

        _html.erase(cmd.startPos, cmd.endPos - cmd.startPos);
        auto resultHtml = readFile(baseFile);
        if (resultHtml.empty()) {
            return {cmd.endPos, std::make_unique<nodes::HtmlNode>("Parse error: extends command extends empty file.")};
        }

        BlocksEvaluator::combine(_html, resultHtml);

        _html.clear();

        return {0, std::make_unique<nodes::ExtendsNode>(resultHtml)};
    }
}
