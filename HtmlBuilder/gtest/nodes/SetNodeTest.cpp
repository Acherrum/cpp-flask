#include "gtest/gtest.h"

#include "cppflask/html/nodes/SetNode.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::nodes {

class SetNodeTest : public testing::Test {
public:
    JsonObject data{R"raw({})raw"};
};

TEST_F(SetNodeTest, SimpleNumericAssignment_Positive) {
    auto node = SetNode("x", "5.0");

    node.render(data);

    auto result = data.getValue("x", -1.0);
    ASSERT_DOUBLE_EQ(5.0, result);
}


TEST_F(SetNodeTest, SimpleNumericAssignment_Negative) {
    auto node = SetNode("x", "-5");

    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0L);
    ASSERT_EQ(value, -5L);
}


TEST_F(SetNodeTest, SimpleTextAssignment) {
    auto node = SetNode("x", "\"hello\"");

    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", std::string{});
    ASSERT_EQ(value, "hello");
}


TEST_F(SetNodeTest, SimpleOperation_Addition) {
    auto node = SetNode("x", "5+1");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 6UL);
}


TEST_F(SetNodeTest, SimpleOperation_Subtract) {
    auto node = SetNode("x", "5-1");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 4UL);
}


TEST_F(SetNodeTest, SimpleOperation_Multiply) {
    auto node = SetNode("x", "5*2");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 10UL);
}


TEST_F(SetNodeTest, SimpleOperation_Division) {
    auto node = SetNode("x", "5/2");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0.0);
    ASSERT_DOUBLE_EQ(value, 2.5);
}


TEST_F(SetNodeTest, SimpleOperation_Modulo) {
    auto node = SetNode("x", "5%2");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", -1337L);
    ASSERT_EQ(value, 1);
}


TEST_F(SetNodeTest, OrderOfOperations_NegativePlusPositive) {
    auto node = SetNode("x", "-5+3");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", -1337L);
    ASSERT_EQ(value, -2);
}


TEST_F(SetNodeTest, OrderOfOperations_NegativeTimesNegativeIsPositive) {
    auto node = SetNode("x", "-5*-1");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", -1337L);
    ASSERT_EQ(value, 5);
}

TEST_F(SetNodeTest, OrderOfOperations_MultiplicationBeforeAddition) {
    auto node = SetNode("x", "2*3+5*3");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 21UL);
}

TEST_F(SetNodeTest, OrderOfOperations_BracesGetPrecedence) {
    auto node = SetNode("x", "(3+5)*(1+1)");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 16UL);
}

TEST_F(SetNodeTest, OrderOfOperations_ModuloBeforeMultiplication) {
    auto node = SetNode("x", "3*3%2*2");
    
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 2UL);
}

TEST_F(SetNodeTest, OrderOfOperations_VariableIsSubstituted) {
    auto node = SetNode("x", "$i*2");
    
    data.add("i",3UL);
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 6UL);
}

TEST_F(SetNodeTest, OrderOfOperations_AllVariablesAreSubstituted) {
    auto node = SetNode("x", "$i*$j");
    
    data.add("i",3UL);
    data.add("j",3UL);
    node.render(data);

    // Verify the value was set correctly
    auto value = data.getValue("x", 0UL);
    ASSERT_EQ(value, 9UL);
}

}
