#pragma once

#include <hopscotch_map.h>
#include "file/file_manager.h"
#include "injector/injectable.h"
#include "graphics/texture.h"
#include "memory/spinlock.h"

namespace nebula {

    class TextureLoader : public Injectable {
        Spinlock::Lock _lock;

        hmap<string, Texture> _textures;

        FileManager* _fileManager{nullptr};

    public:
        TextureLoader() = default;

        Texture load(const TexProps& tProp);

        void free(Texture& tex);

        NEBULA_INJECTABLE(TextureLoader);
    };
}
