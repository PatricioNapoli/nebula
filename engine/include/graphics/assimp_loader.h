#pragma once

#include "memory/containers.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <logging/logger.h>

#include "scene/render_component.h"

namespace nebula {
    class AssimpLoader {
        Logger* _logger{nullptr};

    public:
        AssimpLoader() = default;
        explicit AssimpLoader(Logger* logger) : _logger(logger) {}

        void load(const string& filepath, RenderComponent& model);

    private:
        void processNode(unsigned int numNode, aiNode* node, const aiScene* scene, RenderComponent& model);
        RenderComponent::Mesh processMesh(aiMesh* mesh, const aiScene* scene, RenderComponent& model);
        void setMaterialTexturesPath(Material& material, aiMaterial* mat, aiTextureType type, Texture::Type ourType, RenderComponent& model);
    };
}
