#include "cppflask/Logger.h"

#include <vector>
#include <fstream>
#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>
#include <format>
#include <chrono>

namespace {
    class Logging {
    public:
        ~Logging() {
            _logger.log(cppflask::LogLevel::Debug, "Shutting down logging.");
              
            auto isDone = false;
            while(!isDone) {
                {
                    std::scoped_lock lock{_logmutex}; 
                    isDone = _logs.empty();
                }
                if (!isDone) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
            }
            _logging = false;
            _logThread.join();
        }

        static void log(std::string text) {
            static Logging store;
            std::scoped_lock lock{store._logmutex};
            store._logs.push_back(std::move(text));
        }


        
    private:
        Logging() : _logThread{&Logging::logThings, this} {}
        cppflask::Logger _logger{"Logging FW"};
        bool _logging{true};
        std::thread _logThread;
        std::mutex _logmutex{};
        std::vector<std::string> _logs{};

        void logThings() {
            _logger.log(cppflask::LogLevel::Debug, "Starting logging.");

            while(_logging) {
                auto local = std::vector<std::string>{};
                auto logfile = std::ofstream{"output.log", std::ios_base::app};
                auto retries = 0;
                if (!logfile.is_open()) {
                    _logger.log(cppflask::LogLevel::Error, "Not able to open log file.");
                    retries++;
                    if (retries == 5) {
                        _logging = false;
                    }
                } else {
                    {
                        std::size_t count = 0;
                        std::scoped_lock lock{_logmutex};                        
                        for (auto logIter = _logs.begin(); logIter != _logs.end() && count < 50; count++) {
                            std::cout << *logIter;
                            logfile << *logIter;
                            logIter = _logs.erase(logIter);
                        }
                    }
                    std::cout << std::flush;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
    };
}

namespace cppflask {
Logger::Logger(const std::string& name) : _name{name} {}

Logger::~Logger() = default;

void Logger::log(LogLevel level, const std::string& text) const {
    if (DEFAULT_LOG_LEVEL <= level) {
        std::stringstream ss;
        auto now = std::chrono::time_point_cast<std::chrono::seconds>(
             std::chrono::system_clock::now()
           );
        auto time = std::format("{:%F %T}", now);
        ss << std::left << std::setw(20) << time << std::left << std::setw(10) << level << std::left << std::setw(15) << _name << std::left << text << "\n";
        Logging::log(ss.str());
    }
}
}