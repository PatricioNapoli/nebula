#pragma once

#include <chrono>
#include "memory/containers.h"
#include <iomanip>
#include <sstream>

namespace nebula {
    class SystemTime {
        typedef std::chrono::system_clock SChrono;

    public:
        [[nodiscard]] string getToday() const;
        [[nodiscard]] time_t getEpoch() const;
    };
}
