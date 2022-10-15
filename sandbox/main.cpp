#include "engine.h"

#include <iostream>

#include "injector/injector.h"
#include "window/window.h"
#include "scene/entity.h"
#include "graphics/shapes.h"
#include "scene/transform_component.h"
#include "memory/ptr.h"

int main() {
    auto engine = new nebula::Engine(nebula::Config{
            "nebula::Engine",
            "res/config.yml",
            nebula::config::Standalone,
            nebula::config::debug::OFF,
            &std::cout});

    engine->init();

    auto window = engine->getInjector()->getService<nebula::Window>();
    auto graphics = engine->getInjector()->getService<nebula::OpenGL>();
    auto shaderLoader = engine->getInjector()->getService<nebula::ShaderLoader>();
    auto cameraSystem = engine->getInjector()->getService<nebula::Camera>();
    auto ecs = engine->getInjector()->getService<nebula::ECS>();

    //window->showCursor();

    //graphics->enablePoly();

    graphics->loadShader("skybox_default", "res/shaders/skybox.vert", "res/shaders/skybox.frag");
    graphics->loadShader("main", "res/shaders/main.vert", "res/shaders/main.frag");
    graphics->loadShader("pbr", "res/shaders/pbr.vert", "res/shaders/pbr.frag");

    auto main = shaderLoader->getShader("main");
    auto pbr = shaderLoader->getShader("pbr");
    auto skybox = shaderLoader->getShader("skybox_default");

    auto* scene = engine->addScene("main scene");
    auto* backpack = engine->addEntity("backpack", scene);
    auto* cam = engine->addEntity("cam", scene);

    auto entt = nebula::Reference<nebula::Entt>(scene);
    ecs->pushEntity(entt);

    auto& camera = cam->addComponent<nebula::CameraComponent>(
            "default",
            glm::vec3(0.0, 0.0, 5.0),
            glm::vec3(0.0, 0.0, 0.0),
            45.0,
            0.05,
            0.001,
            0.1f,
            1000.0f
    );

    auto camRef = nebula::Reference<nebula::Component>(camera);
    ecs->pushComponent<nebula::CameraComponent>(camRef, entt);
    auto test = ecs->view<nebula::CameraComponent>();

    cameraSystem->setCurrent(&camera);

    backpack->addComponent<nebula::RenderComponent>("res/models/backpack/backpack.obj", main);
    auto& backT = backpack->addComponent<nebula::TransformComponent>(glm::vec3(0.0, -2.0, 0.0), glm::vec3(0.0), glm::vec3(1.0));
    backT.size(glm::vec3(0.5));
    backT.move(glm::vec3(3.0f, -2.0f, 0.0));
    //backT.rotateY(glm::radians(180.0f));

    for (int i = 0; i < 16; i += 2) {
        auto* sphere = engine->addEntity(nebula::string("sphere" + std::to_string(i)), scene);
        auto v = {nebula::shapes::sphere(64, 64)};
        sphere->addComponent<nebula::RenderComponent>(nebula::RenderComponent::PRIMITIVE, pbr, v);
        sphere->addComponent<nebula::TransformComponent>(glm::vec3(i - 8, 3.0, 0.0), glm::vec3(0.0), glm::vec3(1.0));
    }

    engine->setScene(scene);

    graphics->loadLights(main);
    graphics->loadLights(pbr);

    graphics->loadCubemap("res/models/Skybox", skybox);

    {
        NEBULA_PROFILE;
        engine->update();
    }

    delete engine;
}
