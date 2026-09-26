#include "cppflask/html/parsers/MacroRegistry.h"


namespace cppflask::html::parsers {
    MacroRegistry::MacroRegistry() = default;
    
    void MacroRegistry::save(MacroDefinition macro) {

        get()._registry.emplace(macro.name, macro);
    }

    std::optional<MacroDefinition> MacroRegistry::get(const std::string &name)
    {
        auto macroIter = get()._registry.find(name);
        if (macroIter == get()._registry.end()) {
            return std::nullopt;
        }

        return macroIter->second;
    }

    void MacroRegistry::clear() {

        get()._registry.clear();
    }

    MacroRegistry &MacroRegistry::get() {
        static MacroRegistry _instance;
        return _instance;
    }


}