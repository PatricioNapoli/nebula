#include "graphics/opengl/opengl_shader.h"

#include <GL/glew.h>

#include "logging/logger.h"

namespace nebula::opengl {
    bool OpenGLShader::load() {
        NEBULA_PROFILE;

        id = glCreateProgram();

        if (!compileShader(vert, GL_VERTEX_SHADER)) {
            return false;
        }

        if (!compileShader(frag, GL_FRAGMENT_SHADER)) {
            return false;
        }

        glLinkProgram(id);
        glValidateProgram(id);
        return true;
    }

    bool OpenGLShader::compileShader(const char* source, unsigned int type) {
        auto shader = glCreateShader(type);

        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint result;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE) {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            vector<char> error(length);
            glGetShaderInfoLog(shader, length, &length, &error[0]);
            glDeleteShader(shader);
            return false;
        }

        glAttachShader(id, shader);
        glDeleteShader(shader);

        return true;
    }

    void OpenGLShader::use() {
        glUseProgram(id);
    }

    void OpenGLShader::setInt(const string& unif, int n) {
        glUniform1i(uniform(unif), n);
    }

    void OpenGLShader::setFloat(const string& unif, float n) {
        glUniform1f(uniform(unif), n);
    }

    void OpenGLShader::setVec3(const string& unif, const glm::vec3& vec) {
        glUniform3f(uniform(unif), vec.x, vec.y, vec.z);
    }

    void OpenGLShader::setVec3(const string& unif, float x, float y, float z) {
        glUniform3f(uniform(unif), x, y, z);
    }

    void OpenGLShader::setMat4(const string& unif, const glm::mat4& mat) {
        glUniformMatrix4fv(uniform(unif), 1, GL_FALSE, &mat[0][0]);
    }

    int OpenGLShader::uniform(const string& uniform) {
        if (uniformLocations.find(uniform) != uniformLocations.end()) {
            return uniformLocations[uniform];
        }

        int uLocation = glGetUniformLocation(id, uniform.c_str());
        uniformLocations[uniform] = uLocation;

        return uLocation;
    }
}
