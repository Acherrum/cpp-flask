#pragma once

#include <vector>
#include <memory>

namespace cppflask {
class JsonObject;

namespace html {
namespace nodes {
    class Node;
}

class HtmlBuilder {
public:
    static HtmlBuilder fromFile(const std::string& path);
    static HtmlBuilder fromText(const std::string& html);

    static std::unique_ptr<HtmlBuilder> uniqueFromFile(const std::string& path);
    static std::unique_ptr<HtmlBuilder> uniqueFromText(const std::string& html);

    ~HtmlBuilder();

    HtmlBuilder(const HtmlBuilder&) = delete;
    HtmlBuilder(HtmlBuilder&&) = default;
    HtmlBuilder& operator=(const HtmlBuilder&) = delete;
    HtmlBuilder& operator=(HtmlBuilder&&) = default;

    std::string buildWithData(JsonObject& data) const;
    [[nodiscard]] std::string build() const;

private:
    HtmlBuilder(std::string name, std::vector<std::unique_ptr<nodes::Node>>);

    std::string _name;
    std::vector<std::unique_ptr<nodes::Node>> _nodes;
};
}
}
