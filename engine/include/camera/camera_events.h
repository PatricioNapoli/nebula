#pragma once

#include "memory/containers.h"

namespace nebula::camera {
    struct CameraUpdateMsg : messaging::Message {
        NEBULA_EVENT_NAME("camera.update")

        CameraUpdateMsg() = default;
    };
}
