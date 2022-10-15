#include "config/configuration.h"

#include <GL/glew.h>

#include "file/file_utils.h"

#define NEBULA_DEFAULT_CONFIG \
"windowWidth: 1024\n" \
"windowHeight: 768\n" \
"multisample: yes\n" \
"borderless: no\n" \
"graphics_api: opengl\n" \
"vsync: yes\n"

namespace nebula {

    void Configuration::mapDependencies(EnvironmentVars& globalEnv) {
        _config = std::any_cast<Config*>(globalEnv["config"]);

        map<FileManager>(&_fileManager);
    }

    void Configuration::onConstruct() {
        if (_config->configPath.empty() || !file::exists(_config->configPath)) {
            _fileManager->write(_config->configPath, NEBULA_DEFAULT_CONFIG);
            _fileConfig = YAML::LoadFile(_config->configPath.c_str());
        }
        else {
            _fileConfig = YAML::LoadFile(_config->configPath.c_str());

            if (_fileConfig.Type() == YAML::NodeType::Map) {
                auto def = YAML::Load(NEBULA_DEFAULT_CONFIG);
                for (auto const& entry : def) {
                    _fileConfig[entry.first] = entry.second;
                }
            }
        }

        _config->windowWidth = _fileConfig["windowWidth"].as<GLint>();
        _config->windowHeight = _fileConfig["windowHeight"].as<GLint>();

        _config->multisample = _fileConfig["multisample"].as<bool>();
        _config->borderless = _fileConfig["borderless"].as<bool>();

        _config->vsync = _fileConfig["vsync"].as<bool>();
    }
}
