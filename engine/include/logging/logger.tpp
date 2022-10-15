namespace nebula {
    template<typename... Args>
    void Logger::trace(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::trace))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stdoutLogger->trace(f.data(), args...);
        _rotatingLogger->trace(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::debug(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::debug))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stdoutLogger->debug(f.data(), args...);
        _rotatingLogger->debug(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::info(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::info))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stdoutLogger->info(f.data(), args...);
        _rotatingLogger->info(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::warn(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::warn))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stdoutLogger->warn(f.data(), args...);
        _rotatingLogger->warn(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::error(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::err))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stderrLogger->error(f.data(), args...);
        _rotatingLogger->error(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::critical(const string& fmt, const Args &... args) {
        if (!shouldLog(spdlog::level::critical))
            return;

        string f = buildFmt(fmt);
        if (_output == nullptr)
            _stderrLogger->critical(f.data(), args...);
        _rotatingLogger->critical(f.data(), args...);

        logStream(fmt.data(), args...);
    }

    template<typename... Args>
    void Logger::logStream(const char* fmt, const Args &... args) {
        if (_output == nullptr)
            return;

        *_output << string_utils::format("{}{}", getStreamFmt().data(), string_utils::format(fmt, args...).data()).data() << std::endl;
    }
}
