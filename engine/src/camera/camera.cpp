#include "camera/camera.h"

#include "input/input_events.h"
#include "camera/camera_events.h"
#include "window/events.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nebula {
    void Camera::mapDependencies(EnvironmentVars& globalEnv) {
        map<Input>(&_input);
        map<Configuration>(&_configuration);
        map<Messaging>(&_messaging);
        map<DeltaTimer>(&_deltaTimer);
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
    }

    void Camera::onConstruct() {
        _messaging->subscribe<input::KeyMsg>([&](const input::KeyMsg& m) {
            updateKeys();
        });

        _messaging->subscribe<input::CursorMsg>([&](const input::CursorMsg& m) {
            updateMouse();
        });

        _messaging->subscribe<window::ResizeMsg>([&](const window::ResizeMsg& m) {
            _width = m.width;
            _height = m.height;

            for (auto& cam : _scene->getComponent<CameraComponent>()) {
                cam->projectionMatrix = glm::perspective(
                        glm::radians(cam->fov),
                        float(_width) / float(_height),
                        cam->zNear,
                        cam->zFar
                );
            }

            updateMVP();
        });

        _messaging->subscribe<engine::SceneChangedMsg>([&](const engine::SceneChangedMsg& s) {
            _scene = s.scene;

            findCam();
        });

        _width = _configuration->get().windowWidth;
        _height = _configuration->get().windowHeight;
    }

    void Camera::update() {
        NEBULA_PROFILE;

        updateKeys();
    }

    void Camera::updateKeys() {
        auto updated = false;

        if (_input->isKeyPressed(GLFW_KEY_W)) {
            _current->position += _current->direction * _current->movementSpeed * (float)_deltaTimer->delta();
            updated = true;
        }

        if (_input->isKeyPressed(GLFW_KEY_S)) {
            _current->position -= _current->direction * _current->movementSpeed * (float)_deltaTimer->delta();
            updated = true;
        }

        if (_input->isKeyPressed(GLFW_KEY_D)) {
            _current->position += _current->right * _current->movementSpeed * (float)_deltaTimer->delta();
            updated = true;
        }

        if (_input->isKeyPressed(GLFW_KEY_A)) {
            _current->position -= _current->right * _current->movementSpeed * (float)_deltaTimer->delta();
            updated = true;
        }

        if (updated) {
            updateMVP();
        }
    }

    void Camera::updateMouse() {
        auto mouseOffsets = _input->getMouseOffsets();

        _current->horizontalRadians += static_cast<float>(_current->mouseSensitivity * mouseOffsets.x);
        _current->verticalRadians += static_cast<float>(_current->mouseSensitivity * mouseOffsets.y);

        if (_current->verticalRadians > 1.6)
            _current->verticalRadians = 1.6;
        else if (_current->verticalRadians < -1.6)
            _current->verticalRadians = -1.6;

        _current->direction = glm::vec3(
                cos(_current->verticalRadians) * sin(_current->horizontalRadians),
                sin(_current->verticalRadians),
                cos(_current->verticalRadians) * cos(_current->horizontalRadians)
        );

        _current->right = glm::vec3(
                sin(_current->horizontalRadians - 3.14 / 2.0),
                0,
                cos(_current->horizontalRadians - 3.14 / 2.0)
        );

        _current->up = cross(_current->right, _current->direction);

        updateMVP();
    }

    void Camera::updateMVP() {
        updateMVP(_current);
        _messaging->publish(camera::CameraUpdateMsg());
    }

    void Camera::updateMVP(CameraComponent* cam) {
        NEBULA_PROFILE;

        cam->viewMatrix = lookAt(
                cam->position,
                cam->position + cam->direction,
                cam->up
        );

        cam->VP = cam->projectionMatrix * cam->viewMatrix;
        cam->VPI = cam->projectionMatrix * glm::mat4(glm::mat3(cam->viewMatrix));
    }

    void Camera::setCurrent(CameraComponent* camera) {
        if (_current != nullptr) {
            _current->active = false;
        }

        camera->active = true;
        _current = camera;
        load();
    }

    CameraComponent* Camera::getCurrent() {
        if (_current == nullptr) {
            findCam();
        }

        return _current;
    }

    void Camera::load() {
        NEBULA_PROFILE;

        _current->direction = glm::vec3(
                cos(_current->verticalRadians) * sin(_current->horizontalRadians),
                sin(_current->verticalRadians),
                cos(_current->verticalRadians) * cos(_current->horizontalRadians)
        );

        _current->right = glm::vec3(
                sin(_current->horizontalRadians - 3.14 / 2.0),
                0,
                cos(_current->horizontalRadians - 3.14 / 2.0)
        );

        _current->up = cross(_current->right, _current->direction);

        auto viewMatrix = lookAt(
                _current->position,
                _current->position + _current->direction,
                _current->up
        );

        // TODO config
        auto projectionMatrix = glm::perspective(
                glm::radians(_current->fov),
                float(_width) / float(_height),
                _current->zNear,
                _current->zFar
        );

//        auto projectionMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 1000.0f);

        _current->viewMatrix = viewMatrix;
        _current->projectionMatrix = projectionMatrix;

        _current->VP = projectionMatrix * viewMatrix;
        _current->VPI = projectionMatrix * glm::mat4(glm::mat3(viewMatrix));
    }

    void Camera::findCam() {
        for (auto& cam : _scene->getComponent<CameraComponent>()) {
            if (cam->active) {
                _current = cam;
                load();
                break;
            }
        }
    }
}
