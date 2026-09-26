#pragma once

#include "cppflask/html/nodes/BaseNode.h"

#include <string>
#include <memory>

#include "cppflask/html/HtmlBuilder.h"
#include "cppflask/html/parsers/loop/ExpressionEvaluator.h"

namespace cppflask::html::nodes {

class LoopNode : public BaseNode {
public:
    LoopNode(parsers::loop::LoopSettings settings, HtmlBuilder contents);

    ~LoopNode() override;

    std::string render(JsonObject& data) const override;

private:
    parsers::loop::LoopSettings _settings;
    HtmlBuilder _contents;

    long long getLoopStart(JsonObject& data) const;
    long long getLoopEnd(JsonObject& data) const;
};

}
