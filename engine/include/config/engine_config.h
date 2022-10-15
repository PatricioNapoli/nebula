#pragma once

#include "memory/containers.h"
#include <sstream>

namespace nebula {

    namespace config {
        enum RunMode {
            Standalone,
            Editor
        };

        namespace debug {
            enum Mode {
                ON,
                OFF
            };
        }
    }

    struct Config {
        string appName;
        string configPath;
        config::RunMode mode{config::Standalone};
        config::debug::Mode debug{config::debug::OFF};

        std::ostream* streamOutput;

        int windowWidth{1024};
        int windowHeight{768};

        bool multisample{true};
        bool borderless{false};

        int vsync{1};

        Config(string appName, string configPath, config::RunMode mode, config::debug::Mode debug, std::ostream *loggerOutput) :
                appName(std::move(appName)), configPath(std::move(configPath)), mode(mode), debug(debug), streamOutput(loggerOutput) {}
    };
}
