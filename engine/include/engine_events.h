#pragma once

#include "concurrency/messages.h"

namespace nebula {
    class Scene;
}

namespace nebula::engine {

    struct SceneChangedMsg : public messaging::Message {
        NEBULA_EVENT_NAME(engine.scenechanged)

        Scene* scene{nullptr};

        explicit SceneChangedMsg(Scene* scene) : scene(scene) {}
    };
}
