#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "ecs/component.h"
#include "templating/type_hash.h"

namespace nebula {
    class TransformComponent : public Component {

    public:
        glm::vec3 position{0.0f};
        glm::vec3 eulerAngles{1.0f};
        glm::vec3 scale{1.0f};

        glm::mat4 modelMatrix{1.0f};

        TransformComponent(const glm::vec3& pos, const glm::vec3& eulerZXY, const glm::vec3& scl) : position(pos), eulerAngles(eulerZXY), scale(scl) {
            transform(position, eulerZXY, scale);
        }

        void transform(const glm::vec3 pos, const glm::vec3 eulerAnglesZXY, const glm::vec3 scl) {
            position = pos;
            scale = scl;
            eulerAngles = eulerAnglesZXY;

            glm::mat4 translation = glm::translate(glm::mat4(1.0), position);
            glm::mat4 rot = glm::orientate3(eulerAngles);
            glm::mat4 scaling = glm::scale(glm::mat4(1.0), scale);

            modelMatrix = translation * rot * scaling;
        }

        void move(const glm::vec3& pos) {
            transform(pos, eulerAngles, scale);
        }

        void size(const glm::vec3& scl) {
            transform(position, eulerAngles, scl);
        }

        void rotateZYX(const glm::vec3& euler) {
            transform(position, euler, scale);
        }

        void rotateX(float x) {
            transform(position, glm::vec3(x, eulerAngles.y, eulerAngles.z), scale);
        }

        void rotateY(float y) {
            transform(position, glm::vec3(eulerAngles.x, y, eulerAngles.z), scale);
        }

        void rotateZ(float z) {
            transform(position, glm::vec3(eulerAngles.x, eulerAngles.y, z), scale);
        }

    private:
        float clampRad() {
            //auto pi = glm::pi<float>();
            return 0.0f;
        }

        glm::mat4 safeQuatLookAt(const glm::vec3& lookTo) {
            return glm::mat4();
        }
    };
}
