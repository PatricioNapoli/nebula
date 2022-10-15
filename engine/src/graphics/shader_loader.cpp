#include "graphics/shader_loader.h"

namespace nebula {
    void ShaderLoader::mapDependencies(EnvironmentVars& globalEnv) {
        map<Logger>(&_logger);
        map<FileManager>(&_fileManager);
    }

    void ShaderLoader::onConstruct() {

    }
}
