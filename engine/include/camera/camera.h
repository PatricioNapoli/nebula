#pragma once

#include <glm/glm.hpp>
#include <utility>

#include "time/delta_timer.h"
#include "scene/camera_component.h"
#include "input/input.h"
#include "injector/injectable.h"
#include "scene/scene.h"
#include "engine_events.h"

namespace nebula {
    class Camera : public Injectable {
    private:
        Input* _input{nullptr};
        Logger* _logger{nullptr};
        Configuration* _configuration{nullptr};
        Messaging* _messaging{nullptr};
        DeltaTimer* _deltaTimer{nullptr};
        Scene* _scene{nullptr};

        int _width{1024};
        int _height{768};

    public:
        void update();

        void setCurrent(CameraComponent* camera);
        CameraComponent* getCurrent();

    NEBULA_INJECTABLE(Camera);

    private:
        void updateKeys();
        void updateMouse();
        void updateMVP();
        void updateMVP(CameraComponent* cam);

        CameraComponent* _current{nullptr};
        void load();
        void findCam();
    };
}
