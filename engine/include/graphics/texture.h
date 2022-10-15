#pragma once

#include <unordered_map>
#include "memory/containers.h"
#include <utility>

#include "serialization/serializable.h"

namespace nebula {
    struct Texture {
        unsigned int id{};
        unsigned char* data{};

        int width{};
        int height{};
        int component{};

        string path{};
        bool alpha{false};

        enum Type : uint8_t {
            DIFFUSE,
            SPECULAR,
            AMBIENT,
            REFLECTION,
            EMISSIVE,
            HEIGHT,
            OPACITY,
            NORMALS,
            METALNESS,
            SHININESS,
            EMISSION_COLOR,
            DIFFUSE_ROUGHNESS,
            AMBIENT_OCCLUSSION,
            CUBEMAP,
            NONE,
            UNKNOWN
        } type{UNKNOWN};

        static hmap<Type, string> enumMappings;

        string typeString;

        Texture() : type(UNKNOWN) {
            typeString = enumMappings[UNKNOWN];
        }

        Texture(Type type, string path) : path(std::move(path)), type(type) {
            typeString = enumMappings[type];
        }
    };

    struct TexProps : public Serializable<TexProps> {
        Texture::Type type{};
        string path{};

        TexProps() = default;
        TexProps(Texture::Type type, string path) : type(type), path(std::move(path)) {}

    NEBULA_SERIALIZABLE(TexProps, .tprop, 1, &path, &type);
    };
}
