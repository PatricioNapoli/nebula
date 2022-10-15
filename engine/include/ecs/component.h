#pragma once

#include "templating/crtp.h"
#include "memory/containers.h"

#include <iostream>

namespace nebula {
    class Entity;

    struct Component {
        Entity* entity{nullptr};

        Component() = default;

        NEBULA_CRTP
    };
}
