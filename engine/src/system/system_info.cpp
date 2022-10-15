#include "system/system_info.h"

namespace nebula {
    void SystemInfo::mapDependencies(EnvironmentVars& globalEnv) {
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
        map<Configuration>(&_config);
    }

    void SystemInfo::onConstruct() {
        _systemInfo.cpuModel = getCpuModel();

        if (_config->getOrDefault<std::string>("graphics_api", "") == "opengl") {
            _systemInfo.gpuModel = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
            _systemInfo.openGl = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        }

        init();
    }
}
