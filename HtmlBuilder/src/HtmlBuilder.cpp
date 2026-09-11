#include "cppflask/html/HtmlBuilder.h"

#include <fstream>
#include <iostream>

#include "cppflask/html/nodes/Node.h"
#include "cppflask/html/nodes/HtmlNode.h"

#include "cppflask/html/parsers/conditional/ConditionalsParser.h"
#include "cppflask/html/parsers/loop/LoopParser.h"
#include "cppflask/html/parsers/setter/SetParser.h"
#include "cppflask/html/parsers/simple/IncludeParser.h"
#include "cppflask/html/parsers/simple/VariablesParser.h"
#include "cppflask/html/HtmlCommand.h"

#include "cppflask/JsonObject.h"
#include "cppflask/html/StringHelper.h"

using cppflask::JsonObject;
using cppflask::html::HtmlCommand;
using cppflask::html::parsers::conditional::ConditionalsParser;
using cppflask::html::parsers::loop::LoopParser;
using cppflask::html::parsers::setter::SetParser;
using cppflask::html::parsers::IncludeParser;
using cppflask::html::parsers::VariablesParser;
using cppflask::html::nodes::Node;
using cppflask::html::nodes::HtmlNode;

namespace {
    std::string loadFromFile(const std::string &path) {

        auto file = std::ifstream{path};
        if (!file.is_open()) {
            return "Could not open file: " + path;
        }

        auto html = std::string{};
        auto line = std::string{};
        while (std::getline(file, line)) {
            html += line;
        }
        file.close();

        return html;
    }

    std::vector<std::unique_ptr<Node>> parseHtml(std::string html) {

        std::vector<std::unique_ptr<Node>> nodes{};
        auto startCommandPos = html.find('{');

        while (startCommandPos != std::string::npos) {
            // confirm we have a command or variable
            auto nextChar = html.at(startCommandPos + 1);
            if (nextChar != '{' && nextChar != '%') {
                // we do not, find the next occurance
                startCommandPos = html.find('{', startCommandPos + 1);
                continue;
            }

            // there is some html before the command, put that in an html node
            if (startCommandPos > 0) {
                nodes.emplace_back(std::make_unique<HtmlNode>(html.substr(0, startCommandPos)));
                html.erase(0, startCommandPos);
                if (startCommandPos != std::string::npos) {
                    startCommandPos = 0;
                }
            }

            if (nextChar == '{') {
                auto endCommandPos = html.find("}}", startCommandPos) + 2;
                auto command = html.substr(startCommandPos, endCommandPos-startCommandPos);
                auto cmd = HtmlCommand{command, startCommandPos, endCommandPos};
                auto [endOfParsedData, node] = VariablesParser{html}.parse(cmd);
                nodes.emplace_back(std::move(node));
                html.erase(startCommandPos, endOfParsedData - startCommandPos);
            } else {
                auto endCommandPos = html.find("%}", startCommandPos) + 2;
                auto command = html.substr(startCommandPos, endCommandPos-startCommandPos);
                auto cmd = HtmlCommand{command, startCommandPos, endCommandPos};
                if (command.find("IF") != std::string::npos) {
                    auto [endOfParsedData, node] = ConditionalsParser{html}.parse(cmd);
                    nodes.emplace_back(std::move(node));
                    html.erase(startCommandPos, endOfParsedData - startCommandPos);
                } else if (command.find("FOR") != std::string::npos) {
                    auto [endOfParsedData, node] = LoopParser{html}.parse(cmd);
                    nodes.emplace_back(std::move(node));
                    html.erase(startCommandPos, endOfParsedData - startCommandPos);
                } else if (command.find("SET") != std::string::npos) {
                    auto [endOfParsedData, node] = SetParser{html}.parse(cmd);
                    nodes.emplace_back(std::move(node));
                    html.erase(startCommandPos, endOfParsedData - startCommandPos);
                } else if (command.find("INCLUDE") != std::string::npos) {
                    auto [endOfParsedData, node] = IncludeParser{html}.parse(cmd);
                    if (node != nullptr) {
                        nodes.emplace_back(std::move(node));
                        html.erase(startCommandPos, endOfParsedData - startCommandPos);
                    }
                }
            }
            startCommandPos = html.find('{');
        }

        if (!html.empty()) {
            nodes.emplace_back(std::make_unique<HtmlNode>(html));
        }

        return nodes;
    }
}

namespace cppflask::html {
HtmlBuilder::HtmlBuilder(std::string name, std::vector<std::unique_ptr<Node>> nodes) :
    _name{std::move(name)},
    _nodes{std::move(nodes) } {}

HtmlBuilder::~HtmlBuilder() = default;

HtmlBuilder HtmlBuilder::fromFile(const std::string &path) {

    return {path, parseHtml(loadFromFile(path))};
}

HtmlBuilder HtmlBuilder::fromText(const std::string &html) {

    return {"from text", parseHtml(html)};
}

std::unique_ptr<HtmlBuilder> HtmlBuilder::uniqueFromFile(const std::string &path) {

    return std::unique_ptr<HtmlBuilder>{new HtmlBuilder(path, parseHtml(loadFromFile(path)))};
}

std::unique_ptr<HtmlBuilder> HtmlBuilder::uniqueFromText(const std::string &html) {

    return std::unique_ptr<HtmlBuilder>{new HtmlBuilder("from text", parseHtml(html))};
}

std::string HtmlBuilder::buildWithData(JsonObject &data) const {

    auto result = std::string{};
    for (const auto& node : _nodes) {
        result += node->render(data);
    }
    return result;
}

std::string HtmlBuilder::build() const
{
    auto emptyData = JsonObject{"{}"};
    return buildWithData(emptyData);
}
}
