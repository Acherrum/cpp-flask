#include "cppflask/html/parsers/setter/SetParser.h"

#include "cppflask/html/HtmlCommand.h"
#include "cppflask/html/nodes/HtmlNode.h"
#include "cppflask/html/nodes/SetNode.h"
#include "cppflask/html/StringHelper.h"


namespace cppflask::html::parsers::setter {

    std::pair<std::size_t, std::unique_ptr<nodes::BaseNode>> SetParser::parse(std::string&, const HtmlCommand& cmd) {
        // Extract the content between {% SET and %}
        std::string content = cmd.cmd;
        stripAll(content);
        content = content.substr(find(content, "SET") + 3, content.length()-7);

        // Parse "variable = expression"
        size_t eqPos = content.find('=');
        if (eqPos == std::string::npos) {
            return {cmd.endPos, std::make_unique<nodes::HtmlNode>("")};
        }

        std::string varName = content.substr(0, eqPos);
        std::string expr = content.substr(eqPos + 1);

        if (varName.empty() || expr.empty()) {
            return {cmd.endPos, std::make_unique<nodes::HtmlNode>("")};
        }

        return {cmd.endPos, std::make_unique<nodes::SetNode>(varName, expr)};
    }
}
