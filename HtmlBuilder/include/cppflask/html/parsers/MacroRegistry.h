#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include "cppflask/html/parsers/MacroDefinition.h"

namespace cppflask::html::parsers {
    class MacroRegistry {
    public:

        static void save(MacroDefinition macro);
        static std::optional<MacroDefinition> get(const std::string& name);  
        static void clear();

        MacroRegistry(const MacroRegistry&) = delete;
        MacroRegistry& operator=(const MacroRegistry&) = delete;
        MacroRegistry(MacroRegistry&&) = delete;
        MacroRegistry& operator=(MacroRegistry&&) = delete;

    private:
        std::unordered_map<std::string, MacroDefinition> _registry{};

        static MacroRegistry& get();
        MacroRegistry();
    };
}
