#include "graphics/object_loader.h"

#include "serialization/persistence.h"

namespace nebula {
    void ObjectLoader::mapDependencies(EnvironmentVars &globalEnv) {
        map<FileManager>(&_fileManager);
        map<Logger>(&_logger);
    }

    void ObjectLoader::onConstruct() {
        _assimpLoader = AssimpLoader(_logger);
    }

    void ObjectLoader::load(const string& filepath, RenderComponent& model) {
        auto ext = string(std::filesystem::path(filepath).extension().c_str());

        if (ext == string(".nobj")) {
            Persistence p(model, _fileManager, filepath);
            p.load();
        } else {
            _assimpLoader.load(filepath, model);
        }
    }
}
