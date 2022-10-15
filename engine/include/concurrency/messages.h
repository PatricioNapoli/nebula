#pragma once

#include "memory/containers.h"

#define NEBULA_EVENT_NAME(n) static constexpr auto name = #n;

namespace nebula::messaging {
    struct Message {
        Message() = default;
    };
}
