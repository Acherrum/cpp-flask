#pragma once

#include "cppflask/html/nodes/Node.h"

namespace cppflask::html::nodes {

class BaseNode : public Node {
public:
    BaseNode(const std::string& type, const std::string& html);
    ~BaseNode() override;

    [[nodiscard]] const std::string& getType() const override;

protected:
    std::string _html;
    std::string _type;
};

}