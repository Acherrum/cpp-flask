#include "gtest/gtest.h"

#include "cppflask/html/parsers/conditional/ConditionEvaluator.h"
#include "cppflask/JsonObject.h"

namespace cppflask::html::parsers::conditional {
class ConditionEvaluatorTest : public testing::Test {};

TEST_F(ConditionEvaluatorTest, BooleanExpression) {

    ASSERT_TRUE(ConditionEvaluator{"$truth"}.evaluate(JsonObject{R"raw({"truth":true})raw"}));
}

TEST_F(ConditionEvaluatorTest, BooleanExpression_Inverted) {

    ASSERT_FALSE(ConditionEvaluator{"!$truth"}.evaluate(JsonObject{R"raw({"truth":true})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_booleanIsCastToString) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == true"}.evaluate(JsonObject{R"raw({"truth":true})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_StringLiteral) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == world"}.evaluate(JsonObject{R"raw({"truth":"world"})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_DoubleLiteral) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == 3.141500"}.evaluate(JsonObject{R"raw({"truth":3.1415})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_IntegerLiteral) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == -31415"}.evaluate(JsonObject{R"raw({"truth":-31415})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_IntegerLiteral_GreaterThan) {

    ASSERT_TRUE(ConditionEvaluator{"$truth > -31415"}.evaluate(JsonObject{R"raw({"truth":-31414})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareStrings_IntegerLiteral_SmallerThan) {

    ASSERT_TRUE(ConditionEvaluator{"$truth < -31415"}.evaluate(JsonObject{R"raw({"truth":-31416})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareTypedData_Strings) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == $val"}.evaluate(JsonObject{R"raw({"truth":"hello","val":"hello"})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareTypedData_Double) {

    ASSERT_TRUE(ConditionEvaluator{"$truth < $val"}.evaluate(JsonObject{R"raw({"truth":3.1,"val":3.2})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareTypedData_Double_NotEqual) {

    ASSERT_TRUE(ConditionEvaluator{"$truth != $val"}.evaluate(JsonObject{R"raw({"truth":3.2001,"val":3.2002})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareTypedData_Integer_LT) {

    ASSERT_TRUE(ConditionEvaluator{"$truth < $val"}.evaluate(JsonObject{R"raw({"truth":-42,"val":1337})raw"}));
}

TEST_F(ConditionEvaluatorTest, CompareTypedData_Integer_Equal) {

    ASSERT_TRUE(ConditionEvaluator{"$truth == $val"}.evaluate(JsonObject{R"raw({"truth":-42,"val":-42})raw"}));
}
}