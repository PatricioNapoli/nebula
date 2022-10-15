#pragma once

#include "injector/injectable.h"
#include "engine_config.h"
#include "file/file_manager.h"

#include <yaml-cpp/yaml.h>

namespace nebula {

    class Configuration : public Injectable {
        Config* _config{nullptr};
        FileManager* _fileManager{nullptr};

        YAML::Node _fileConfig;

    public:
        Config& get() {
            return *_config;
        }

        // TODO dot notation path search
        template <typename T>
        T getOrDefault(const string& path, const T dflt) {
            if (_fileConfig[path.c_str()].IsDefined()) {
                return _fileConfig[path.c_str()].as<T>();
            }
            return dflt;
        }

    NEBULA_INJECTABLE(Configuration);
    };
}
