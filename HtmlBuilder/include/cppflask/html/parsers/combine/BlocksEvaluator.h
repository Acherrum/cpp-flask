#pragma once

#include <string>
#include <map>

namespace cppflask::html::parsers {
    class BlocksEvaluator {
    public:
        static void combine(const std::string& blocksDescription, std::string& target);
    
    private:
        BlocksEvaluator() = default;
        std::multimap<std::string, std::string> _blocks{};

        void readBlocks(const std::string& html);
    };
}