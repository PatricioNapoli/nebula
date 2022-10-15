#include <utility>
#include <iostream>

#include "logging/logger.h"

#include "config/engine_config.h"
#include "file/file_utils.h"
#include "random/random.h"

namespace nebula {
    void Logger::mapDependencies(EnvironmentVars& globalEnv) {
        map<Configuration>(&_config);
    }

    void Logger::onConstruct() {
        _output = _config->get().streamOutput;

        setComponent(_logConfig.component);

        _rotatingLogger = spdlog::get("file");

        if (_rotatingLogger != nullptr) {
            _stdoutLogger = spdlog::get("stdout");
            _stderrLogger = spdlog::get("stderr");
        } else {
            _rotatingLogger = spdlog::rotating_logger_mt("file", "nebula_logs.txt", 1048576 * 5, 3);
            _stdoutLogger = spdlog::stdout_color_mt("stdout");
            _stderrLogger = spdlog::stderr_color_mt("stderr");
        }

        if (_config->get().debug == config::debug::Mode::ON) {
            setLevel(spdlog::level::debug);
            setGlobalLevel(spdlog::level::debug);
        }

        if (_logConfig.component == "engine") {
            Logger::_sessionId = Random::randomAlphaStringG(8);

            info("Session ID: {}", Logger::_sessionId);
        }
    }

    void Logger::setComponent(const string& component) {
        _component = file::getLastFilePart(component);
    }

    void Logger::setPattern(const string& pattern) {
        _rotatingLogger->set_pattern(pattern.c_str());
        _stdoutLogger->set_pattern(pattern.c_str());
        _stderrLogger->set_pattern(pattern.c_str());
    }

    void Logger::setLevel(spdlog::level::level_enum level) {
        _level = level;
    }

    void Logger::setGlobalLevel(spdlog::level::level_enum level) {
        _rotatingLogger->set_level(level);
        _stdoutLogger->set_level(level);
        _stderrLogger->set_level(level);
    }

    string Logger::buildFmt(const string& fmt) {
        return string_utils::format("[{}] {}", _component.data(), fmt.data());
    }

    string Logger::getStreamFmt() {
        return string_utils::format("[{}] [stream] [{}] [{}] ", _time.getToday().data(), spdlog::level::level_string_views[_level].data(), _component.data());
    }

    bool Logger::shouldLog(spdlog::level::level_enum level) {
        return level >= _level;
    }
}
