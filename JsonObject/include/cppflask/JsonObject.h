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

    JsonObject(std::unique_ptr<PImpl> pimpl);

    JsonObject(const JsonObject&);
    JsonObject(JsonObject&&) noexcept;
    JsonObject& operator=(const JsonObject&);
    JsonObject& operator=(JsonObject&&) noexcept;

    std::string getValue(const std::string& key, const std::string& defaultReturnValue) const;
    long getValue(const std::string& key, long defaultReturnValue) const;
    unsigned long getValue(const std::string& key, unsigned long defaultReturnValue) const;
    double getValue(const std::string& key, double defaultReturnValue) const;
    bool getValue(const std::string& key, bool defaultReturnValue) const;

    JsonObject get(const std::string& key);

    std::string getValueAsString(const std::string& key) const;

    std::size_t getArraySize(const std::string& key = "") const;

    bool compareKeys(const std::string& key1, const std::string& key,
                     const std::function<bool(double,double)>& numericComp,
                     const std::function<bool(const std::string&,const std::string&)>& stringComp,
                     const std::function<bool(bool, bool)>& booleanComp) const;

    std::string toString() const;

    void add(const std::string& key, long value);
    void add(const std::string& key, unsigned long value);
    void add(const std::string& key, double value);
    void add(const std::string& key, bool value);
    void add(const std::string& key, const char* value);
    void add(const std::string& key, const std::string& value);

private:
    std::unique_ptr<PImpl> _pimpl{nullptr};
};
}
