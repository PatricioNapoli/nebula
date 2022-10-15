#pragma once

#include <utility>
#include <injector/injection.h>

#include "GL/glew.h"

#include "logging/logger.h"
#include "config/configuration.h"
#include "exception/engine_exception.h"
#include "concurrency/messaging.h"
#include "camera/camera.h"
#include "graphics/object_loader.h"
#include "concurrency/thread_pool.h"
#include "scene/scene.h"
#include "graphics/opengl/opengl_shader.h"
#include "graphics/texture_loader.h"
#include "graphics/shader_loader.h"

namespace nebula {
    class OpenGL : public Injectable {
        Logger* _logger{nullptr};
        Configuration* _config{nullptr};
        DeltaTimer* _deltaTimer{nullptr};
        Messaging* _messaging{nullptr};
        Camera* _camera{nullptr};
        FileManager* _fileManager{nullptr};
        ObjectLoader* _objectLoader{nullptr};
        Scheduler* _scheduler{nullptr};
        ShaderLoader* _shaderLoader{nullptr};

    public:
        OpenGL() = default;

        void draw();
        void clear();
        void update();
        void enablePoly();
        void disablePoly();
        void load(RenderComponent& rc);
        void loadShader(const string& name, const string& vertPath, const string& fragPath);
        void loadTexture(Texture& tex);
        void loadCubemap(const string& dir, IShader* shader);
        void loadLights(IShader* shader);
        void reload();

    NEBULA_INJECTABLE(OpenGL);

    private:
        void init();
        void loadMeshes(RenderComponent& rc);
        void loadCubemapTextures(const string& dir);
        void updateSpotlights();
        void loadComponent(RenderComponent& model);

        Scene* _scene{nullptr};

        TextureLoader _textureLoader;

        hmap<string, bool> _loadedComponents;

        GLuint _skyboxVAO{};
        GLuint _skyboxTexture{0};
        opengl::OpenGLShader* _skyboxShader{};

        opengl::OpenGLShader* _currentShader{};
        void drawSkybox();
    };
}
