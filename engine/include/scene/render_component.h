#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "memory/containers.h"
#include "graphics/shader.h"
#include "graphics/material.h"
#include "serialization/serializable.h"
#include "vector/vector_utils.h"
#include "ecs/component.h"
#include "templating/type_hash.h"

namespace nebula {
    class RenderComponent : public Component, public Serializable<RenderComponent> {

    public:
        struct Vertex {
            glm::vec3 position{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            Vertex() = default;

            explicit Vertex(const glm::vec3 &position) : position(position) {}

            Vertex(const glm::vec3 &position, const glm::vec2 &uv) : position(position), uv(uv) {}

            Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &uv) : position(position),
                                                                                              normal(normal), uv(uv) {}

        };

        struct Mesh : public Serializable<Mesh> {
            unsigned int index{};

            vector<Vertex> vertices{};
            vector<unsigned int> indices{};

            Material material{};

            GLuint VAO{};
            GLuint VBO{};
            GLuint EBO{};

            Mesh() = default;

            Mesh(vector<Vertex>&& vertices, vector<unsigned int>&& indices, Material material = Material()) : material(std::move(material)) {
                vector_utils::move(vertices, this->vertices);
                vector_utils::move(indices, this->indices);
            }

            Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, Material material = Material()) : vertices(vertices), indices(indices), material(std::move(material)) {
            }

        NEBULA_SERIALIZABLE(Mesh, .nmesh, 1, &index, &vertices, &indices, &material);
        };

        enum Type {
            COMPLEX,
            PRIMITIVE,
            SKYBOX
        } type{COMPLEX};

        string dir{};
        string filepath{};

        IShader* shader{nullptr};

        unsigned int verticesCount{};
        unsigned int indicesCount{};

        void setFilePath(const string& fp) {
            dir = fp.substr(0, fp.find_last_of('/') + 1);
            filepath = fp;
        }

        RenderComponent(const string &filepath,
                IShader *shader) :
                shader(shader) {
            setFilePath(filepath);
        }

        RenderComponent(Type type, IShader *shader,
                        vector<Mesh>& meshes)
                : type(type), shader(shader), meshes(meshes) {
        }

        RenderComponent(Type type, IShader *shader,
                        vector<Mesh>&& meshes)
                : type(type), shader(shader) {
            vector_utils::move(meshes, this->meshes);
        }

        vector<Mesh> meshes{};

    NEBULA_SERIALIZABLE(RenderComponent, .nobj, 1, &dir, &filepath, &meshes);
    };
}
