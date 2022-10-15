#pragma once

#include <utility>
#include <sstream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "time/system_time.h"
#include "injector/injectable.h"
#include "config/configuration.h"

#include "string/string_utils.h"

namespace nebula {
    class Logger : public Injectable {
    public:
        struct Config {
            string component;

            Config() : component("global") {}
            explicit Config(string c) : component(std::move(c)) {}
        } _logConfig;

    private:
        Configuration* _config{nullptr};

        spdlog::level::level_enum _level{spdlog::level::level_enum::info};

        string _component{"global"};

        std::shared_ptr<spdlog::logger> _stdoutLogger;
        std::shared_ptr<spdlog::logger> _stderrLogger;
        std::shared_ptr<spdlog::logger> _rotatingLogger;

        std::atomic<std::ostream*> _output{nullptr};

        string _sessionId;

    public:
        Logger() = default;

        explicit Logger(Logger::Config&& c) : _logConfig(std::move(c)), _level(spdlog::level::info), _time() {}
        explicit Logger(Logger::Config& c) : _logConfig(c), _level(spdlog::level::info), _time() {}

        void setComponent(const string& component);
        void setPattern(const string& pattern);
        void setLevel(spdlog::level::level_enum level);

        void setGlobalLevel(spdlog::level::level_enum level);

        string& sessionId() {
            return _sessionId;
        }

        template<typename... Args>
        void trace(const string& fmt, const Args &... args);

        template<typename... Args>
        void debug(const string& fmt, const Args &... args);

        template<typename... Args>
        void info(const string& fmt, const Args &... args);

        template<typename... Args>
        void warn(const string& fmt, const Args &... args);

        template<typename... Args>
        void error(const string& fmt, const Args &... args);

        template<typename... Args>
        void critical(const string& fmt, const Args &... args);
    private:
        bool shouldLog(spdlog::level::level_enum level);

        template<typename... Args>
        void logStream(const char* fmt, const Args &... args);

        string buildFmt(const string& fmt);
        string getStreamFmt();

        SystemTime _time;

    NEBULA_INJECTABLE(Logger);
    };
}

#include "logger.tpp"
