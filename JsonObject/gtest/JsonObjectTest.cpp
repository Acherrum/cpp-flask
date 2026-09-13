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
    object.set("count", 42L);
    ASSERT_EQ(42L, object.getValue("count", -1L));
}

TEST_F(JsonObjectTest, AddUnsignedLongValue) {

    auto object = JsonObject(R"raw({})raw");
    unsigned long id = 999999UL;
    object.set("id", id);
    ASSERT_EQ(id, object.getValue("id", 1UL));
}

TEST_F(JsonObjectTest, AddDoubleValue) {

    auto object = JsonObject(R"raw({})raw");
    object.set("pi", 3.14159);
    ASSERT_DOUBLE_EQ(3.14159, object.getValue("pi", -1.0));
}

TEST_F(JsonObjectTest, AddBoolValue) {

    auto object = JsonObject(R"raw({})raw");
    object.set("active", true);
    ASSERT_TRUE(object.getValue("active", false));
}

TEST_F(JsonObjectTest, DealWithNestedObjects) {

    const auto object = JsonObject(R"raw({"nested": { "name": "val" }})raw");
    auto nested = object.get("nested");
    ASSERT_EQ("val", nested.getValue("name", std::string{}));
    nested.set(std::string{"new"}, 123L);
    ASSERT_EQ(123L, object.getValue("nested/new", -1L));
    ASSERT_EQ(R"raw({"name":"val","new":123})raw", nested.toString());
}

    TEST_F(JsonObjectTest, DealWithNestedArrays) {

    const auto object = JsonObject(R"raw({"nested":[{"id":1},{"id":2},{"id":3}]})raw");
    {
        auto array = object.get("nested");
        for (long i = 0; i < array.getArraySize(); i++) {
            auto nested = array.get(std::to_string(i));
            nested.set(std::string{"index"}, i);
        }
    }
    ASSERT_EQ(R"raw({"nested":[{"id":1,"index":0},{"id":2,"index":1},{"id":3,"index":2}]})raw", object.toString());
}

TEST_F(JsonObjectTest, EmptyObject) {
    const auto object = JsonObject("{}");
    ASSERT_TRUE(object.isEmpty());
}

TEST_F(JsonObjectTest, EmptyArray) {
    const auto object = JsonObject("[]");
    ASSERT_TRUE(object.isEmpty());
}

TEST_F(JsonObjectTest, AddObject) {
    auto object1 = JsonObject("{}");
    {
        const auto object2 = JsonObject("{\"id\": 1}");
        ASSERT_TRUE(object1.isEmpty());
        object1.set("nested", object2);
    }
    ASSERT_FALSE(object1.isEmpty());
    ASSERT_EQ(1L, object1.getValue("nested/id", -1L));
}

TEST_F(JsonObjectTest, AddObjectIntoArray) {
    auto object1 = JsonObject("{}");
    {
        const auto object2 = JsonObject("{\"id\": 1}");
        ASSERT_TRUE(object1.isEmpty());
        object1.set("nested/0", object2);
    }
    ASSERT_FALSE(object1.isEmpty());
    ASSERT_EQ(1L, object1.getValue("nested/0/id", -1L));
    ASSERT_EQ(R"raw({"nested":[{"id":1}]})raw", object1.toString());
}

TEST_F(JsonObjectTest, UpdateValue) {

    auto object = JsonObject(R"raw({"hello":"world"})raw");
    object.set("hello",std::string{"Peter"});
    ASSERT_EQ("Peter", object.getValue("hello", std::string{}));
}

TEST_F(JsonObjectTest, UpdateValuesInAnArrayUsingSet) {
    auto object = JsonObject(R"raw({"nested":[1,2,3,4]})raw");
    {
        auto nested = object.get("nested");
        ASSERT_EQ(2L, nested.getValue("1", -1L));
        nested.set("1", -5L);
        ASSERT_EQ(R"raw([1,-5,3,4])raw", nested.toString());
    }
    object.set("nested/2", 6L);
    ASSERT_EQ(R"raw({"nested":[1,-5,6,4]})raw", object.toString());
}

TEST_F(JsonObjectTest, ClearArrayUsingSetMethod) {
    auto object = JsonObject(R"raw({"nested":[1,2,3,4]})raw");
    ASSERT_EQ(4, object.getArraySize("nested"));
    object.set("nested", JsonObject("[]"));
    ASSERT_EQ(0, object.getArraySize("nested"));
    ASSERT_EQ(R"raw({"nested":[]})raw", object.toString());
}

}
