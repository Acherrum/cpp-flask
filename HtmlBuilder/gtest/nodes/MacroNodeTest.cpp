#include "gtest/gtest.h"

#include "cppflask/html/nodes/MacroNode.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {

class MacroNodeTest : public testing::Test {};

TEST_F(MacroNodeTest, rendersTheBodyOfTheMacro_NoArguments) {
    auto node = MacroNode{"Hello, world!", {}, {}};
    auto data = JsonObject{};
    ASSERT_EQ("Hello, world!", node.render(data));
}

TEST_F(MacroNodeTest, rendersTheBodyOfTheMacro_Arguments) {
    auto node = MacroNode{"{{ $message }}", {"message" }, { "Hello, world!" }};
    auto data = JsonObject{};
    ASSERT_EQ("Hello, world!", node.render(data));
}
}