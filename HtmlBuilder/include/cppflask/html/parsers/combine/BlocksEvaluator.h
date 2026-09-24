#pragma once

#include <string>

namespace cppflask::html::parsers {
    class BlocksEvaluator {
    public:
        static void combine(const std::string& blocksDescription, std::string& target);
    };
}