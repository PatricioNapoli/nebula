#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "graphics/texture.h"
#include "serialization/serializable.h"
#include "vector/vector_utils.h"

namespace nebula {
    struct Material : public Serializable<Material> {
        string name{};

        vector<Texture> textures{};
        vector<TexProps> texProps{};

        glm::vec3 albedo{0.5f, 0.0f, 0.0f};
        float metallic{0.8f};
        float roughness{0.3f};
        float ambientOcclusion{1.0f};

        Material() = default;

        explicit Material(string name) : name(std::move(name)) {}

        Material(string name, vector<Texture>&& textures) : name(std::move(name)) {
            vector_utils::move(textures, this->textures);
        }

        Material(string name, vector<TexProps>& texProps) : name(std::move(name)), texProps(texProps) {
        }

        NEBULA_SERIALIZABLE(Material, .nmat, 1, &name, &texProps, &albedo, &metallic, &roughness, &ambientOcclusion);
    };
}
