#pragma once

#include <string>
#include <memory>
#include <functional>

namespace cppflask {
struct PImpl;

class JsonObject {
public:
    JsonObject();
    explicit JsonObject(const std::string& jsonString);
    ~JsonObject();

    explicit JsonObject(std::unique_ptr<PImpl> pimpl);

    JsonObject(const JsonObject&);
    JsonObject(JsonObject&&) noexcept;
    JsonObject& operator=(const JsonObject&);
    JsonObject& operator=(JsonObject&&) noexcept;

    [[nodiscard]] std::string getValue(const std::string& key, const std::string& defaultReturnValue) const;
    [[nodiscard]] long getValue(const std::string& key, long defaultReturnValue) const;
    [[nodiscard]] unsigned long getValue(const std::string& key, unsigned long defaultReturnValue) const;
    [[nodiscard]] double getValue(const std::string& key, double defaultReturnValue) const;
    [[nodiscard]] bool getValue(const std::string& key, bool defaultReturnValue) const;

    [[nodiscard]] JsonObject get(const std::string& key) const;

    [[nodiscard]] std::string getValueAsString(const std::string& key) const;

    [[nodiscard]] std::size_t getArraySize(const std::string& key = "") const;

    bool compareKeys(const std::string& key1, const std::string& key,
                     const std::function<bool(double,double)>& numericComp,
                     const std::function<bool(const std::string&,const std::string&)>& stringComp,
                     const std::function<bool(bool, bool)>& booleanComp) const;

    [[nodiscard]] std::string toString() const;

    void add(const std::string& key, long value);
    void add(const std::string& key, unsigned long value);
    void add(const std::string& key, double value);
    void add(const std::string& key, bool value);
    void add(const std::string& key, const char* value);
    void add(const std::string& key, const std::string& value);
    void add(const std::string& key, const JsonObject& object);

    [[nodiscard]] bool isEmpty() const;

private:
    std::unique_ptr<PImpl> _pimpl{nullptr};
};
}
