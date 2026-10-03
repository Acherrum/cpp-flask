#pragma once

#include <string>
#include <ostream>
#include <unordered_map>

namespace cppflask {

enum class LogLevel {
    Trace, Debug, Warning, Info, Error
};

inline LogLevel DEFAULT_LOG_LEVEL = LogLevel::Info;

inline std::ostream& operator<<(std::ostream& out, LogLevel level) {
    static std::unordered_map<LogLevel, std::string> levelMap{
        {LogLevel::Trace, "TRACE"},
        {LogLevel::Debug, "DEBUG"},
        {LogLevel::Warning, "WARN"},
        {LogLevel::Info, "INFO"},
        {LogLevel::Error, "ERROR"},
    };
    auto levelIter = levelMap.find(level);
    if (levelIter == levelMap.end()) {
        out << "UNKNOWN";
    } else {
        out << levelIter->second;
    }
    return out;
}

class Logger {
public:
    explicit Logger(const std::string& name);
    ~Logger();

    void log(LogLevel level, const std::string& ss) const;

private:
    std::string _name;
};
}