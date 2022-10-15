#include "graphics/assimp_loader.h"

#include <iostream>
#include <profiling/profiler.h>
#include "graphics/material.h"

namespace nebula {
    void AssimpLoader::load(const string& filepath, RenderComponent& model) {
        NEBULA_PROFILE_D(filepath);

        Assimp::Importer import;
        const auto* scene = import.ReadFile(filepath.c_str(), aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices );

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            _logger->error("Failed to load model at: {}", filepath);
            return;
        }

        processNode(0, scene->mRootNode, scene, model);
    }

    void AssimpLoader::processNode(unsigned int numNode, aiNode* node, const aiScene* scene, RenderComponent& model) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            auto* mesh = scene->mMeshes[node->mMeshes[i]];
            auto id = i + numNode;
            auto m = processMesh(mesh, scene, model);
            m.index = id;
            model.meshes.emplace_back(m);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(i, node->mChildren[i], scene, model);
        }
    }

    RenderComponent::Mesh AssimpLoader::processMesh(aiMesh* mesh, const aiScene* scene, RenderComponent& model) {
        vector<RenderComponent::Vertex> vertices{};
        vertices.reserve(mesh->mNumVertices);

        vector<unsigned int> indices{};
        indices.reserve(mesh->mNumFaces * 3);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec3 vertex;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;

            glm::vec3 normals;
            normals.x = mesh->mNormals[i].x;
            normals.y = mesh->mNormals[i].y;
            normals.z = mesh->mNormals[i].z;

            glm::vec2 uv;
            if (mesh->mTextureCoords[0])
            {
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
            }
            else
                uv = glm::vec2(0.0f, 0.0f);

            vertices.emplace_back(vertex, normals, uv);
        }

        model.verticesCount += vertices.size();

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);

            model.indicesCount += 3;
        }

        RenderComponent::Mesh m{vertices, indices};

        if (mesh->mMaterialIndex >= 0)
        {
            auto material = scene->mMaterials[mesh->mMaterialIndex];
            hmap<aiTextureType, Texture::Type> toLoad = {
                    {aiTextureType_DIFFUSE, Texture::DIFFUSE},
                    {aiTextureType_SPECULAR, Texture::SPECULAR},
                    {aiTextureType_EMISSIVE, Texture::EMISSIVE},
                    {aiTextureType_AMBIENT, Texture::AMBIENT},
                    {aiTextureType_HEIGHT, Texture::HEIGHT},
                    {aiTextureType_AMBIENT_OCCLUSION, Texture::AMBIENT_OCCLUSSION},
                    {aiTextureType_NORMALS, Texture::NORMALS},
                    {aiTextureType_METALNESS, Texture::METALNESS},
                    {aiTextureType_DIFFUSE_ROUGHNESS, Texture::DIFFUSE_ROUGHNESS},
                    {aiTextureType_OPACITY, Texture::OPACITY},
                    {aiTextureType_NONE, Texture::NONE},
                    {aiTextureType_UNKNOWN, Texture::UNKNOWN},
            };

            for (auto& t : toLoad) {
                setMaterialTexturesPath(m.material, material, t.first, t.second, model);
            }
        }

        return m;
    }

    void AssimpLoader::setMaterialTexturesPath(Material& m, aiMaterial* mat, aiTextureType type, Texture::Type ourType, RenderComponent& model) {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            auto path = string(str.data);
            string_utils::replace(path, "..\\\\", "");
            string_utils::replace(path, "..\\", "");
            string_utils::replace(path, "\\\\", "/");
            string_utils::replace(path, "\\", "/");

            auto texPath = string(model.dir + path);

            m.texProps.emplace_back(ourType, texPath);
        }
    }
}
