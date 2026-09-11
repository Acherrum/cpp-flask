#pragma once

#include <string>

#include "cppflask/html/parsers/loop/LoopSettings.h"

namespace cppflask::html::parsers::loop {

class ExpressionEvaluator {
public:
    static LoopSettings evaluate(const std::string& expression);

};

}