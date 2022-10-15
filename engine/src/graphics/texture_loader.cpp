#include "graphics/texture_loader.h"

#include "profiling/profiler.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace nebula {
    void TextureLoader::mapDependencies(EnvironmentVars &globalEnv) {
        map<FileManager>(&_fileManager);
    }

    void TextureLoader::onConstruct() {

    }

    Texture TextureLoader::load(const TexProps& tProp) {
        if (_textures.contains(tProp.path)) {
            return _textures.at(tProp.path);
        }

        NEBULA_PROFILE_D(tProp.path);

        auto f = _fileManager->read(tProp.path, FileManager::File::String);

        Texture t{tProp.type, tProp.path};
        t.data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(f.str.c_str()), f.str.size(), &t.width, &t.height, &t.component, t.alpha ? STBI_rgb_alpha : STBI_rgb);

        Spinlock l(_lock);
        _textures.emplace(t.path, t);
        return _textures.at(t.path);
    }

    void TextureLoader::free(Texture& tex) {
        stbi_image_free(tex.data);
    }
}
