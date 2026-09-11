#include "cppflask/html/nodes/LoopNode.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/JsonObject.h"

namespace {
    
std::string replaceLoopVariable(const std::string& loopContent, const std::string& variable, const std::string& newValue) {
    auto loopContentForThisIteration = loopContent;
    auto varPos = loopContentForThisIteration.find("$" + variable);
    while (varPos != std::string::npos) {
        auto endVarPos = varPos + 1 + variable.length();
        if (newValue.at(0) != '$') {
            auto bracesPos = loopContentForThisIteration.rfind("{{", varPos);
            if (bracesPos != std::string::npos && varPos == loopContentForThisIteration.find_first_not_of(' ', bracesPos + 2)) {
                varPos = bracesPos;
                endVarPos = loopContentForThisIteration.find("}}", varPos) + 2;
            }
        }
        loopContentForThisIteration.replace(varPos, endVarPos - varPos, newValue);
        varPos = loopContentForThisIteration.find("$" + variable, varPos + newValue.length());
    }
    return loopContentForThisIteration;
}
}

namespace cppflask::html::nodes {

LoopNode::LoopNode(parsers::loop::LoopSettings settings, std::string contents) :
    BaseNode("Loop",""),
    _settings(std::move(settings)),
    _contents(std::move(contents)) {
}

LoopNode::~LoopNode() = default;

std::string LoopNode::render(JsonObject& data)
{
    auto result = std::string{};

    long long start = 0;
    try {
        start = getLoopStart(data);
    } catch (...) {
        return "Parse error: could not parse loop start: '" + _settings.start + "'";
    }
    long long end = 0;
    try {
        end = getLoopEnd(data);
    } catch (...) {
        return "Parse error: could not parse loop end: '" + _settings.end + "'";
    }

    for (long long i = start; i < end; i++) {
        auto newValue = std::to_string(i);
        if (!_settings.dataVar.empty()) {
            newValue = std::string{"$"}.append(_settings.dataVar).append("/").append(newValue);
        }
        result += HtmlBuilder::fromText(replaceLoopVariable(_contents, _settings.variable, newValue))
            .buildWithData(data);
    }

    return result;
}

long long LoopNode::getLoopStart(JsonObject& data) {
    if (_settings.start.empty()) {
        return 0LL;
    }

    if (_settings.start.at(0) == '$') {
        return std::stol(data.getValueAsString(_settings.start.substr(1)));
    } else {
        return std::stol(_settings.start);
    }
}

long long LoopNode::getLoopEnd(JsonObject &data) {
    if (!_settings.dataVar.empty()) {
        return static_cast<long long>(data.getArraySize(_settings.dataVar));
    }

    if (_settings.end.empty()) {
        return 0LL;
    }

    if (_settings.end.at(0) == '$') {
        return std::stol(data.getValueAsString(_settings.end.substr(1)));
    } else {
        return std::stol(_settings.end);
    }

}
}
