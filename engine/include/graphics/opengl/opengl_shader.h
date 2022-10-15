#pragma once

#include "memory/containers.h"
#include "graphics/shader.h"
#include <glm/glm.hpp>

namespace nebula::opengl {
    struct OpenGLShader : public IShader {
        unsigned int id{0};
        hmap<string, int> uniformLocations{};

        OpenGLShader() = default;

        OpenGLShader(const string& name, const char* vert, const char* frag) : IShader(name, vert, frag) {}

        bool load();
        void use();

        void setInt(const string& unif, int n);
        void setFloat(const string& unif, float n);
        void setVec3(const string& unif, float x, float y, float z);
        void setVec3(const string& unif, const glm::vec3& vec);
        void setMat4(const string& unif, const glm::mat4& mat);

        int uniform(const string& uniform);

        bool compileShader(const char* source, unsigned int type);
    };
}
