#pragma once

#include "memory/containers.h"

#include "injector/injectable.h"
#include "scene/render_component.h"
#include "graphics/assimp_loader.h"

namespace nebula {

    class ObjectLoader : public Injectable {
        AssimpLoader _assimpLoader;

        FileManager* _fileManager{nullptr};
        Logger* _logger{nullptr};

    public:
        ObjectLoader() = default;

        void load(const string& filepath, RenderComponent& model);

        NEBULA_INJECTABLE(ObjectLoader);
    };
}
