#pragma once

#include <string>

namespace cppflask {
class JsonObject;

namespace html::parsers {
class CommandVariablesParser {
public:
    explicit CommandVariablesParser(std::string& html);
    void parse(const JsonObject& data);

private:
    std::string& _html;
};
}
}
