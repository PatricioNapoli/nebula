#pragma once

#include <utility>
#include "logging/logger.h"
#include "graphics/shader.h"
#include "memory/ptr.h"

namespace nebula {

    class ShaderLoader : public Injectable {
        Logger* _logger{nullptr};
        FileManager* _fileManager{nullptr};

        hmap<string, Scope<IShader>> _shaders{};

    public:
        ShaderLoader() = default;

        template <typename T>
        T* loadShader(const string& name, const string& vertPath, const string& fragPath) {
            const char* vert = _fileManager->cache(vertPath, FileManager::File::String).str.c_str();
            const char* frag = _fileManager->cache(fragPath, FileManager::File::String).str.c_str();

            _shaders[name] = Scoped<T>(name, vert, frag);
            return _shaders[name].get()->impl<T>();
        }

        auto& shaders() {
            return _shaders;
        }

        IShader* getShader(const string& name) {
            return _shaders[name].get();
        }

        template <typename T>
        T* getShaderT(const string& name) {
            return _shaders[name].get()->impl<T>();
        }

        NEBULA_INJECTABLE(ShaderLoader);
    };
}
