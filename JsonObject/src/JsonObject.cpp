#include "cppflask/JsonObject.h"

#include "rapidjson/document.h"
#include "rapidjson/pointer.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace cppflask {
struct PImpl {
    PImpl(std::unique_ptr<rapidjson::Document> doc) :
        _root{doc.get()}, _doc{std::move(doc)}, _value{nullptr} {}

    PImpl(rapidjson::Document* root, rapidjson::Value* value) :
        _root{root}, _doc{nullptr}, _value{value} {}

    PImpl(const PImpl& other) : _root{other._root}, _doc{nullptr}, _value{other._value} {}

    [[nodiscard]] rapidjson::Value& get() const {
        if (_value == nullptr) {
            return *_root;
        }
        return *_value;
    }

    [[nodiscard]] rapidjson::Document* getRoot() const {
        return _root;
    }

    rapidjson::Document* _root;
    std::unique_ptr<rapidjson::Value> _doc;
    rapidjson::Value* _value{};
};

namespace {
    std::unique_ptr<rapidjson::Document> generateDocument(const std::string& jsonString) {

        auto doc = std::make_unique<rapidjson::Document>();
        doc->Parse(jsonString.c_str());
        return doc;
    }

    rapidjson::Pointer parseKey(const std::string& input) {
        auto key = std::string{"/"};
        if (!input.empty() && input.at(0) == '/') {
            key = input;
        } else {
            key += input;
        }
        return rapidjson::Pointer{key.c_str()};
    }

    void setMember(PImpl& object, const std::string& key, const rapidjson::Value& value) {
        auto pointer = parseKey(key);
        pointer.Set(object.get(), value, object.getRoot()->GetAllocator());
    }

    rapidjson::Value* getValuePointer(PImpl& object, const std::string &key) {
        return key.empty() ? &object.get() : rapidjson::GetValueByPointer(object.get(), parseKey(key));
    }
}

JsonObject::JsonObject() : _pimpl{std::make_unique<PImpl>(std::make_unique<rapidjson::Document>())} {}

JsonObject::JsonObject(const std::string &jsonString) :
    _pimpl{std::make_unique<PImpl>(generateDocument(jsonString))} {}

JsonObject::~JsonObject() = default;

JsonObject::JsonObject(std::unique_ptr<PImpl> pimpl) : _pimpl{std::move(pimpl)} {}

JsonObject::JsonObject(const JsonObject& other) : _pimpl{std::make_unique<PImpl>(*other._pimpl)} {}

JsonObject::JsonObject(JsonObject&& other) noexcept  : _pimpl{std::move(other._pimpl)} {}

JsonObject& JsonObject::operator=(const JsonObject& other) {
    _pimpl = std::make_unique<PImpl>(*other._pimpl);
    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept {
    _pimpl = std::move(other._pimpl);
    return *this;
}

std::string JsonObject::getValue(const std::string &key, const std::string &defaultReturnValue) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr && value->IsString()) {
        return value->GetString();
    }
    return defaultReturnValue;
}

long JsonObject::getValue(const std::string &key, long defaultReturnValue) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr && value->IsInt64()) {
        return value->GetInt64();
    }
    return defaultReturnValue;
}

unsigned long JsonObject::getValue(const std::string &key, unsigned long defaultReturnValue) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr && value->IsUint64()) {
        return value->GetUint64();
    }
    return defaultReturnValue;
}

double JsonObject::getValue(const std::string &key, double defaultReturnValue) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr && value->IsNumber()) {
        return value->GetDouble();
    }
    return defaultReturnValue;
}

bool JsonObject::getValue(const std::string &key, bool defaultReturnValue) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr && value->IsBool()) {
        return value->GetBool();
    }
    return defaultReturnValue;
}
JsonObject JsonObject::get(const std::string& key) const {

    auto value = getValuePointer(*_pimpl, key);
    if (value != nullptr) {
        return JsonObject{std::make_unique<PImpl>(_pimpl->getRoot(), value)};
    }
    return {};
}

std::string JsonObject::toString() const
{
    auto stringbuffer = rapidjson::StringBuffer{};
    auto writer = rapidjson::Writer{stringbuffer};
    _pimpl->get().Accept(writer);

    return stringbuffer.GetString();
}

std::string JsonObject::getValueAsString(const std::string &key) const
{
    auto value = getValuePointer(*_pimpl, key);

    if (value != nullptr) {
        if (value->IsString()) {
            return value->GetString();
        } else if (value->IsInt64()) {
            return std::to_string(value->GetInt64());
        } else if (value->IsUint64()) {
            return std::to_string(value->GetUint64());
        } else if (value->IsNumber()) {
            return std::to_string(value->GetDouble());
        } else if (value->IsBool()) {
            return (value->GetBool() ? "true" : "false");
        }
    }
    return {};
}

std::size_t JsonObject::getArraySize(const std::string& key) const {

    auto value = getValuePointer(*_pimpl, key);
    if (value != nullptr && value->IsArray()) {
        auto ar = value->GetArray();
        return ar.Size();
    }
    return 0;
}

bool JsonObject::compareKeys(const std::string& key1, const std::string& key2,
                             const std::function<bool(double,double)>& numericComp,
                             const std::function<bool(const std::string&,const std::string&)>& stringComp,
                             const std::function<bool(bool, bool)>& booleanComp) const {

    auto value1 = rapidjson::GetValueByPointer(_pimpl->get(), parseKey(key1));
    auto value2 = rapidjson::GetValueByPointer(_pimpl->get(), parseKey(key2));

    if (value1 != nullptr && value2 != nullptr) {
        if (value1->IsNumber() && value2->IsNumber()) {
            return numericComp(value1->GetDouble(), value2->GetDouble());
        }
        if (value1->IsString() && value2->IsString()) {
            return stringComp(value1->GetString(), value2->GetString());
        }
        if (value1->IsBool() && value2->IsBool()) {
            return booleanComp(value1->GetBool(), value2->GetBool());
        }
    }
    return false;
}

void JsonObject::set(const std::string& key, long value) {
    setMember(*_pimpl, key, rapidjson::Value(static_cast<int64_t>(value)));
}

void JsonObject::set(const std::string& key, unsigned long value) {
    setMember(*_pimpl, key, rapidjson::Value(static_cast<uint64_t>(value)));
}

void JsonObject::set(const std::string& key, double value) {
    setMember(*_pimpl, key, rapidjson::Value(value));
}

void JsonObject::set(const std::string& key, bool value) {
    setMember(*_pimpl, key, rapidjson::Value(value));
}

void JsonObject::set(const std::string& key, const char* value) {
    set(key, std::string(value));
}

void JsonObject::set(const std::string& key, const std::string& value) {
    setMember(*_pimpl, key, rapidjson::Value(value.c_str(), _pimpl->getRoot()->GetAllocator()));
}

void JsonObject::set(const std::string &key, const JsonObject &object) {

    setMember(*_pimpl, key, object._pimpl->get());
}

bool JsonObject::hasMember(const std::string &key) const {

    return _pimpl->get().IsObject() && _pimpl->get().HasMember(key.c_str());
}

bool JsonObject::isEmpty() const {

    if (_pimpl->get().IsObject()) {
        return _pimpl->get().ObjectEmpty();
    }
    if (_pimpl->get().IsArray()) {
        return _pimpl->get().Empty();
    }

    return false;
}
}
