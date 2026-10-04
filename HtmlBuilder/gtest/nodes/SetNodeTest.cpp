#include "gtest/gtest.h"

#include "cppflask/html/nodes/SetNode.h"
#include "cppflask/JsonObject.h"
#include "cppflask/html/FilterRegistry.h"

namespace cppflask::html::nodes {

class SetNodeTest : public testing::Test {
public:
    JsonObject data{R"raw({})raw"};
};

TEST_F(SetNodeTest, SimpleNumericAssignment_Positive) {
    auto node = SetNode("x", "5.0");

    node.render(data);

    ASSERT_DOUBLE_EQ(5.0, data.getValue("x", -1.0));
}


TEST_F(SetNodeTest, SimpleNumericAssignment_Negative) {
    auto node = SetNode("x", "-5");

    node.render(data);

    ASSERT_EQ(-5L, data.getValue("x", 0L));
}


TEST_F(SetNodeTest, SimpleTextAssignment) {
    auto node = SetNode("x", "\"hello\"");

    node.render(data);

    ASSERT_EQ("hello", data.getValue("x", std::string{}));
}


TEST_F(SetNodeTest, SimpleOperation_Addition) {
    auto node = SetNode("x", "5+1");
    
    node.render(data);

    ASSERT_EQ(6UL, data.getValue("x", 0UL));
}


TEST_F(SetNodeTest, SimpleOperation_Subtract) {
    auto node = SetNode("x", "5-1");
    
    node.render(data);

    ASSERT_EQ(4UL, data.getValue("x", 0UL));
}


TEST_F(SetNodeTest, SimpleOperation_Multiply) {
    auto node = SetNode("x", "5*2");
    
    node.render(data);

    ASSERT_EQ(10UL, data.getValue("x", 0UL));
}


TEST_F(SetNodeTest, SimpleOperation_Division) {
    auto node = SetNode("x", "5/2");
    
    node.render(data);

    ASSERT_DOUBLE_EQ(2.5, data.getValue("x", 0.0));
}


TEST_F(SetNodeTest, SimpleOperation_Modulo) {
    auto node = SetNode("x", "5%2");
    
    node.render(data);

    ASSERT_EQ(1, data.getValue("x", -1337L));
}


TEST_F(SetNodeTest, OrderOfOperations_NegativePlusPositive) {
    auto node = SetNode("x", "-5+3");
    
    node.render(data);

    ASSERT_EQ(-2, data.getValue("x", -1337L));
}


TEST_F(SetNodeTest, OrderOfOperations_NegativeTimesNegativeIsPositive) {
    auto node = SetNode("x", "-5*-1");
    
    node.render(data);

    ASSERT_EQ(5, data.getValue("x", -1337L));
}

TEST_F(SetNodeTest, OrderOfOperations_MultiplicationBeforeAddition) {
    auto node = SetNode("x", "2*3+5*3");
    
    node.render(data);

    ASSERT_EQ(21UL, data.getValue("x", 0UL));
}

TEST_F(SetNodeTest, OrderOfOperations_BracesGetPrecedence) {
    auto node = SetNode("x", "(3+5)*(1+1)");
    
    node.render(data);

    ASSERT_EQ(16UL, data.getValue("x", 0UL));
}

TEST_F(SetNodeTest, OrderOfOperations_ModuloBeforeMultiplication) {
    auto node = SetNode("x", "3*3%2*2");
    
    node.render(data);

    ASSERT_EQ(2UL, data.getValue("x", 0UL));
}

TEST_F(SetNodeTest, UsingVariables_VariableIsSubstituted) {
    auto node = SetNode("x", "$i*2");
    
    data.set("i",3UL);
    node.render(data);

    ASSERT_EQ(6UL, data.getValue("x", 0UL));
}

TEST_F(SetNodeTest, UsingVariables_AllVariablesAreSubstituted) {
    auto node = SetNode("x", "$i*$j");
    
    data.set("i",3UL);
    data.set("j",3UL);
    node.render(data);

    ASSERT_EQ(9UL, data.getValue("x", 0UL));
}

TEST_F(SetNodeTest, UsePipe_WithoutFilterResultsInSerialization) {

    auto node = SetNode("x", "$array | getLength ");

    data.set("array/0",1UL);
    data.set("array/1",2UL);
    data.set("array/2",3UL);
    node.render(data);

    ASSERT_EQ("[1,2,3]", data.getValueAsString("x"));
}

TEST_F(SetNodeTest, UsePipe_WithFilterRegisteredResultsInMethodBeingCalled) {

    FilterRegistry::registerFilter("getLength", [](JsonObject& array) {
        return std::to_string(array.getSize());
    });

    auto node = SetNode("x", "$array | getLength ");
    data.set("array/0",std::string{"hello"});
    data.set("array/1",std::string{"cpp"});
    data.set("array/2",std::string{"flask"});
    node.render(data);

    ASSERT_EQ("3", data.getValueAsString("x"));
}

TEST_F(SetNodeTest, MultiPipeMagic) {

    FilterRegistry::registerFilter("sortById", [](JsonObject& array) {
        array.sort(SortOrder::Ascending, "id");
        return array.toString();
    });
    FilterRegistry::registerFilter("getIdsBelow10", [](JsonObject& array) {
        auto newArray = JsonObject{};
        for (int i = 0, index = 0; i < array.getSize(); i++) {
            if (array.getValue(std::to_string(i) + "/id", 0L) < 10) {
                newArray.set("/" + std::to_string(index), array.get(std::to_string(i)));
                index++;
            } else {
                break;
            }
        }
        return newArray.toString();
    });

    auto node = SetNode("x", "$test | sortById | getIdsBelow10");
    auto json = JsonObject(R"raw({"test":[{"id":10,"name":"not this one"},{"id":2,"name":"this one too"},{"id":1,"name":"this one"},{"id":12,"name":"nor this one"}]})raw");
    auto result = node.render(json);

    ASSERT_TRUE(result.empty());
    ASSERT_EQ(R"([{"id":1,"name":"this one"},{"id":2,"name":"this one too"}])", json.getValueAsString("x"));
}

}
