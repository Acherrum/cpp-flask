#include "gtest/gtest.h"

#include "cppflask/JsonObject.h"

namespace cppflask {
class JsonObjectTest : public testing::Test {};

TEST_F(JsonObjectTest, GetStringValue) {

    auto object = JsonObject(R"raw({"hello":"world"})raw");
    ASSERT_EQ("world", object.getValue("hello", std::string{}));
}

TEST_F(JsonObjectTest, GetNestedIntValue) {

    auto object = JsonObject(R"raw({"hello": {"world": 1337}})raw");
    ASSERT_EQ(1337L, object.getValue("hello/world", -1L));
}

TEST_F(JsonObjectTest, GetStringFromArrayValue) {

    auto object = JsonObject(R"raw({"hello": {"world": ["one", "two", "three"]}})raw");
    ASSERT_EQ("two", object.getValue("hello/world/1", std::string{}));
}

TEST_F(JsonObjectTest, AddLongValue) {

    auto object = JsonObject(R"raw({})raw");
    object.add("count", 42L);
    ASSERT_EQ(42L, object.getValue("count", -1L));
}

TEST_F(JsonObjectTest, AddUnsignedLongValue) {

    auto object = JsonObject(R"raw({})raw");
    unsigned long id = 999999UL;
    object.add("id", id);
    ASSERT_EQ(id, object.getValue("id", 1UL));
}

TEST_F(JsonObjectTest, AddDoubleValue) {

    auto object = JsonObject(R"raw({})raw");
    object.add("pi", 3.14159);
    ASSERT_DOUBLE_EQ(3.14159, object.getValue("pi", -1.0));
}

TEST_F(JsonObjectTest, AddBoolValue) {

    auto object = JsonObject(R"raw({})raw");
    object.add("active", true);
    ASSERT_TRUE(object.getValue("active", false));
}

TEST_F(JsonObjectTest, DealWithNestedObjects) {

    auto object = JsonObject(R"raw({"nested": { "name": "val" }})raw");
    auto nested = object.get("nested");
    ASSERT_EQ("val", nested.getValue("name", std::string{}));
    nested.add(std::string{"new"}, 123L);
    ASSERT_EQ(123L, object.getValue("nested/new", -1L));
    ASSERT_EQ(R"raw({"name":"val","new":123})raw", nested.toString());
}

    TEST_F(JsonObjectTest, DealWithNestedArrays) {

    auto object = JsonObject(R"raw({"nested":[{"id":1},{"id":2},{"id":3}]})raw");
    auto array = object.get("nested");
    for (long i = 0; i < array.getArraySize(); i++) {
        auto nested = array.get(std::to_string(i));
        nested.add(std::string{"index"}, i);
    }
    ASSERT_EQ(R"raw({"nested":[{"id":1,"index":0},{"id":2,"index":1},{"id":3,"index":2}]})raw", object.toString());
}

}
