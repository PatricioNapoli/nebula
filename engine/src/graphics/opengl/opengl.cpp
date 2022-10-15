#include "graphics/opengl/opengl.h"

#include "concurrency/messages.h"
#include <glm/ext/matrix_transform.hpp>
#include "window/events.h"
#include "engine_events.h"
#include "profiling/profiler.h"
#include "serialization/persistence.h"
#include "scene/transform_component.h"
#include "graphics/shapes.h"
#include "memory/ptr.h"
#include "scene/entity.h"

namespace nebula {
    void OpenGL::mapDependencies(EnvironmentVars& globalEnv) {
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
        map<Configuration>(&_config);
        map<DeltaTimer>(&_deltaTimer);
        map<Messaging>(&_messaging);
        map<Camera>(&_camera);
        map<FileManager>(&_fileManager);
        map<ObjectLoader>(&_objectLoader);
        map<Scheduler>(&_scheduler);
        map<ShaderLoader>(&_shaderLoader);
    }

    void OpenGL::onConstruct() {
        if (_config->getOrDefault<std::string>("graphics_api", "") != "opengl") {
            return;
        }

        _messaging->subscribe<window::ResizeMsg>([&](const window::ResizeMsg& m) {
            glViewport(0, 0, m.width, m.height);
        });

        _messaging->subscribe<engine::SceneChangedMsg>([&](const engine::SceneChangedMsg& s) {
            _scene = s.scene;

            reload();
        });

        init();
    }

    void OpenGL::init() {
        if (auto err = glewInit(); err != GLEW_OK) {
            _logger->critical("GL Error: {}", glewGetErrorString(err));
            throw EngineException("Failed to initialize openGl.", __FILE__, EngineException::Fatal);
        }

        if (!glewIsSupported("GL_VERSION_4_1")) {
            throw EngineException("openGl version not supported.", __FILE__, EngineException::Fatal);
        }

        //Depth Testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //Face culling
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        if (_config->get().multisample) {
            glEnable(GL_MULTISAMPLE);
            _logger->info("Multisampling on!");
        }

        //Blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGL::loadLights(IShader* shader) {
        auto s = _shaderLoader->getShaderT<opengl::OpenGLShader>(shader->name);
        
        s->use();

        s->setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
        s->setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
        s->setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
        s->setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);

        s->setVec3("pointLights[0].position", glm::vec3( 2.0f,  0.5f,  3.0f));

        s->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        s->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        s->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);

        s->setFloat("pointLights[0].constant", 1.0f);
        s->setFloat("pointLights[0].linear", 0.09);
        s->setFloat("pointLights[0].quadratic", 0.032);

        s->setVec3("pointLights[1].position", glm::vec3( -2.0f,  3.0f,  3.0f));

        s->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        s->setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.5f);
        s->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);

        s->setFloat("pointLights[1].constant", 1.0f);
        s->setFloat("pointLights[1].linear", 0.09);
        s->setFloat("pointLights[1].quadratic", 0.032);

        s->setVec3("spotLight.position", _camera->getCurrent()->position);
        s->setVec3("spotLight.direction", _camera->getCurrent()->direction);

        s->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        s->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        s->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);

        s->setFloat("spotLight.constant", 1.0f);
        s->setFloat("spotLight.linear", 0.09);
        s->setFloat("spotLight.quadratic", 0.032);
        s->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        s->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    }

    void OpenGL::updateSpotlights() {
        _currentShader->setVec3("spotLight.position", _camera->getCurrent()->position);
        _currentShader->setVec3("spotLight.direction", _camera->getCurrent()->direction);
    }

    void OpenGL::draw() {
        if (_scene == nullptr || _camera->getCurrent() == nullptr)
            return;

        NEBULA_PROFILE;

        for (auto& rc : _scene->getComponent<RenderComponent>()) {
            auto& r = *rc;

            if (!_loadedComponents[r.entity->name()])
                continue;

            auto transform = rc->entity->getComponent<TransformComponent>();

            if (transform.empty())
                continue;

            NEBULA_PROFILE_D("RenderComp Start");

            _currentShader = r.shader->impl<opengl::OpenGLShader>();
            _currentShader->use();

            updateSpotlights();

            _currentShader->setMat4("projection", _camera->getCurrent()->projectionMatrix);
            _currentShader->setMat4("view", _camera->getCurrent()->viewMatrix);
            _currentShader->setVec3("ViewPosition", _camera->getCurrent()->position);

            _currentShader->setMat4("model", transform[0]->modelMatrix);

            // Can't seem to use GL_TEXTURE0..
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);
            _currentShader->setInt("skybox", 1);

            transform[0]->rotateY(transform[0]->eulerAngles.y * _deltaTimer->delta() / 8);

            for (auto& mesh : r.meshes) {

                NEBULA_PROFILE_D("RenderMesh Start");

                // TODO: Remake all materials rendering.. need to branch depending on shader type? Shader should declare
                //      dependencies of uniforms at runtime, have uniform buffer objects to serve dependency. Shader Graph?
                _currentShader->setFloat("material.shininess", 32.0f);

                _currentShader->setVec3("albedo", mesh.material.albedo);
                _currentShader->setFloat("metallic", mesh.material.metallic);
                _currentShader->setFloat("roughness", mesh.material.roughness);
                _currentShader->setFloat("ao", mesh.material.ambientOcclusion);

                int i = 2;
                for (auto& tex : mesh.material.textures) {
                    NEBULA_PROFILE_D("RenderMat Start");

                    glActiveTexture(GL_TEXTURE0 + i);
                    glBindTexture(GL_TEXTURE_2D, tex.id);
                    _currentShader->setInt("material." + tex.typeString, i);

                    i++;
                }

                NEBULA_PROFILE_D("Render Elements Start");

                glBindVertexArray(mesh.VAO);
                if (r.type == RenderComponent::COMPLEX) {
                    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
                } else {
                    glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
                }
                glBindVertexArray(0);
            }
        }

        drawSkybox();
    }

    void OpenGL::drawSkybox() {
        if (_skyboxShader == nullptr)
            return;

        NEBULA_PROFILE_D("Render Skybox");

        _skyboxShader->use();

        glDepthFunc(GL_LEQUAL);

        _skyboxShader->setMat4("vp", _camera->getCurrent()->VPI);
        _skyboxShader->setInt("skybox", 0);

        glBindVertexArray(_skyboxVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    void OpenGL::reload() {
        _loadedComponents.clear();

        for (auto& rc : _scene->getComponent<RenderComponent>()) {
            load(*rc);
        }
    }

    void OpenGL::load(RenderComponent& rc) {
        if (rc.filepath.empty()) {
            loadMeshes(rc);
            return;
        }

        auto b = Reference<Briefing>(string_utils::format("Load RenderComponent: %s", rc.filepath), Briefing::SINGLE, [=, &rc]() {
            _objectLoader->load(rc.filepath, rc);

            return std::any();
        });

        b->withCallback([=, &rc](std::any& data){
            loadMeshes(rc);
        })->onMain();

        _scheduler->runAsync(b);
    }

    void OpenGL::loadMeshes(RenderComponent& rc) {
        vector<Ref<Briefing>> chain;

        for (auto& mesh : rc.meshes) {
            for (auto& tProp : mesh.material.texProps) {
                auto load = Reference<Briefing>(string_utils::format("Load Mesh: %s", rc.filepath), Briefing::SINGLE, [=, &tProp, &mesh]() {
                    mesh.material.textures.push_back(_textureLoader.load(tProp));

                    return std::any();
                });

                chain.push_back(load);
            }
        }

        if (chain.empty()) {
            loadComponent(rc);
        } else {
            chain.back()->withCallback([&](std::any& data){
                loadComponent(rc);
            })->onMain();

            _scheduler->runAsync(chain);
        }
    }

    void OpenGL::loadShader(const string& name, const string& vertPath, const string& fragPath) {
        auto shader = _shaderLoader->loadShader<opengl::OpenGLShader>(name, vertPath, fragPath);
        shader->load();
    }

    void OpenGL::loadComponent(RenderComponent& model) {
        NEBULA_PROFILE_D(model.entity->name());

        for (auto& mesh : model.meshes) {
            for (auto& tex : mesh.material.textures) {
                loadTexture(tex);
            }

            glGenVertexArrays(1, &mesh.VAO);
            glGenBuffers(1, &mesh.VBO);
            glGenBuffers(1, &mesh.EBO);

            glBindVertexArray(mesh.VAO);
            glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
            glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(RenderComponent::Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(GLuint),
                         &mesh.indices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderComponent::Vertex),
                                  static_cast<GLvoid*>(nullptr));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderComponent::Vertex),
                                  reinterpret_cast<GLvoid*>(offsetof(RenderComponent::Vertex, normal)));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(RenderComponent::Vertex),
                                  reinterpret_cast<GLvoid*>(offsetof(RenderComponent::Vertex, uv)));

            glBindVertexArray(0);
        }

        _loadedComponents[model.entity->name()] = true;

        if (model.type == RenderComponent::COMPLEX) {
            Persistence<RenderComponent> p(model, _fileManager, "res/models/backpack/backpack.nobj");
            p.persist();
        }
    }

    void OpenGL::loadTexture(Texture& tex) {
        NEBULA_PROFILE_D(tex.path);

        GLuint textureID;
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, tex.alpha ? GL_RGBA : GL_RGB, tex.width, tex.height, 0, tex.alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, tex.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex.alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex.alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        _textureLoader.free(tex);

        tex.id = textureID;
    }

    void OpenGL::loadCubemap(const string& dir, IShader* shader) {

        arc::Ptr<string> sDir = arc::Ptr<string>(dir);
        auto b = Reference<Briefing>(string_utils::format("Load Cubemap: %s", dir), Briefing::SINGLE, [=]() {
            vector<string> faces{"right", "left", "top", "bottom", "back", "front"};

            for (auto &face : faces) {
                _textureLoader.load(TexProps{Texture::CUBEMAP, string_utils::format("{}/{}.jpg", *sDir, face)});
            }

            return std::any();
        });

        b->withCallback([=](std::any& data){
            loadCubemapTextures(dir);

            _skyboxShader = _shaderLoader->getShaderT<opengl::OpenGLShader>(shader->name);

            GLuint skyboxVBO;
            glGenVertexArrays(1, &_skyboxVAO);
            glGenBuffers(1, &skyboxVBO);
            glBindVertexArray(_skyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(shapes::SkyboxVertices), &shapes::SkyboxVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
            glBindVertexArray(0);
        })->onMain();

        _scheduler->runAsync(b);
    }

    void OpenGL::loadCubemapTextures(const string& dir) {
        vector<string> faces{"right", "left", "top", "bottom", "back", "front"};

        glGenTextures(1, &_skyboxTexture);
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, _skyboxTexture);
        auto i = 0;
        for (auto& face : faces) {
            auto t = _textureLoader.load(TexProps{Texture::CUBEMAP, string_utils::format("{}/{}.jpg", dir, face)});

            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                    GL_RGB, t.width, t.height, 0, GL_RGB, GL_UNSIGNED_BYTE, t.data
            );

            _textureLoader.free(t);
            i++;
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGL::clear() {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void OpenGL::update() {
        NEBULA_PROFILE;

        clear();
        draw();
    }

    void OpenGL::enablePoly() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void OpenGL::disablePoly() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
