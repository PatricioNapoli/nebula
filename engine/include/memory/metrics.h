#pragma once

namespace nebula::metrics {
    struct MemoryMetrics {
        size_t usedMemory = 0;

        size_t CurrentUsage() const {
            return usedMemory;
        }
    };

    inline MemoryMetrics Memory;
}
