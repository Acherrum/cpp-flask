#include "cppflask/html/nodes/LoopNode.h"

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {

LoopNode::LoopNode(parsers::loop::LoopSettings settings, HtmlBuilder contents) :
    BaseNode("Loop",""),
    _settings(std::move(settings)),
    _contents(std::move(contents)) {
}

LoopNode::~LoopNode() = default;

std::string LoopNode::render(JsonObject& data) const
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
        auto index = std::to_string(i);
        if (!_settings.dataVar.empty()) {
            data.set(_settings.variable, data.get(_settings.dataVar + "/" + index));
        } else {
            data.set(_settings.variable, index);
        }
        result += _contents.buildWithData(data);
    }

    return result;
}

long long LoopNode::getLoopStart(JsonObject& data) const {
    if (_settings.start.empty()) {
        return 0LL;
    }

    if (_settings.start.at(0) == '$') {
        return std::stol(data.getValueAsString(_settings.start.substr(1)));
    } else {
        return std::stol(_settings.start);
    }
}

long long LoopNode::getLoopEnd(JsonObject &data) const {
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
