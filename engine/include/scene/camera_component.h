#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <GL/glew.h>
#include <vector>

#include "memory/containers.h"
#include "serialization/serializable.h"
#include "ecs/component.h"
#include "templating/type_hash.h"

namespace nebula {
    class CameraComponent : public Component, public Serializable<CameraComponent> {

    public:
        string name;

        bool active{false};

        glm::vec3 position{};
        glm::vec3 direction{};
        glm::vec3 right{};
        glm::vec3 up{};

        glm::mat4 viewMatrix{glm::mat4(1.0)};
        glm::mat4 projectionMatrix{glm::mat4(1.0)};

        glm::mat4 VP{glm::mat4(1.0)};
        glm::mat4 VPI{glm::mat4(1.0)};

        float fov{45.0f};
        float movementSpeed{0.0f};
        float mouseSensitivity{0.0f};

        float zNear{0.1f};
        float zFar{1000.0f};

        double horizontalRadians{glm::radians(180.0)};
        double verticalRadians{0.0};

        enum Type{
           PERSPECTIVE,
           ORTHO
        } type{PERSPECTIVE};

        CameraComponent() = default;
        CameraComponent(string name, const glm::vec3 &position, const glm::vec3 &direction, float fov,
                        float movementSpeed, float mouseSensitivity, float zNear, float zFar) : name(std::move(name)), position(position),
                                                                                                direction(direction), fov(fov), movementSpeed(movementSpeed),
                                                                                                mouseSensitivity(mouseSensitivity), zNear(zNear), zFar(zFar) {}

    NEBULA_SERIALIZABLE(CameraComponent, .ncam, 1);
    };
}
