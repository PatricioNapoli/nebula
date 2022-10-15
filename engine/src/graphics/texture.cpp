#include "graphics/texture.h"

namespace nebula {
    hmap<Texture::Type, string> Texture::enumMappings = {
            {DIFFUSE, "diffuse"},
            {SPECULAR, "specular"},
            {AMBIENT, "ambient"},
            {REFLECTION, "reflection"},
            {EMISSIVE, "emissive"},
            {HEIGHT, "height"},
            {OPACITY, "opacity"},
            {NORMALS, "normals"},
            {METALNESS, "metalness"},
            {SHININESS, "shininess"},
            {EMISSION_COLOR, "emission_color"},
            {DIFFUSE_ROUGHNESS, "diffuse_roughness"},
            {AMBIENT_OCCLUSSION, "ambient_occlussion"},
            {CUBEMAP, "cubemap"},
            {NONE, "none"},
            {UNKNOWN, "unknown"}
    };
}
