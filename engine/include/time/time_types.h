#pragma once

#include <chrono>

namespace nebula {
    typedef std::chrono::high_resolution_clock HRChrono;
    typedef std::chrono::milliseconds Milliseconds;
    typedef std::chrono::microseconds Microseconds;
    typedef std::chrono::nanoseconds Nanoseconds;
}
