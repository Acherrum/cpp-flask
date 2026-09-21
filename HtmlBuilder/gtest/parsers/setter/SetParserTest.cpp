#include "gtest/gtest.h"

#include "cppflask/html/parsers/setter/SetParser.h"
#include "cppflask/html/HtmlCommand.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::setter {

class SetParserTest : public testing::Test {
public:
    JsonObject data{R"raw({})raw"};
};

TEST_F(SetParserTest, SimpleAssignment) {
    // Test simple numeric assignment
    std::string html = R"raw({% SET x = 5 %})raw";
    auto cmd = HtmlCommand{html, 0, html.length()};

    auto [endOfData, node] = SetParser::parse(html, cmd);
    ASSERT_NE(endOfData, std::size_t{0});

    auto result = node->render(data);
    ASSERT_TRUE(result.empty());
    ASSERT_EQ("{\"x\":5}", data.toString());

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 5UL);
}

TEST_F(SetParserTest, MultipleSetAssignmentAndUsages) {
    // Test simple numeric assignment
    std::string html = R"raw({% SET x = 5 %}{% SET y = $x - 1 %})raw";
    auto cmd1 = HtmlCommand{"{% SET x = 5 %}", 0, 15};
    auto cmd2 = HtmlCommand{"{% SET y = $x - 1 %}", 15, 20};

    auto [endOfData1, node1] = SetParser::parse(html, cmd1);
    ASSERT_NE(endOfData1, std::size_t{0});
    auto [endOfData2, node2] = SetParser::parse(html, cmd2);
    ASSERT_NE(endOfData1, std::size_t{0});

    node1->render(data);
    node2->render(data);

    // Verify the value was set correctly
    auto value1 = data.getValue("x", 0UL);
    auto value2 = data.getValue("y", 0UL);
    ASSERT_EQ(value1, 5UL);
    ASSERT_EQ(value2, 4UL);
}

} // namespace cppflask::html::parsers::setter
