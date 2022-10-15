#pragma once

#include "profiling/profiler.h"

namespace nebula {
    struct Spinlock {
        struct Lock {
            std::atomic<bool> lock = {false};
        };
        Lock& _lock;

        explicit Spinlock(Lock& l) noexcept : _lock(l) {
            NEBULA_PROFILE;

            for (;;) {
                // Optimistic assumption
                if (!_lock.lock.exchange(true, std::memory_order_acquire)) {
                    return;
                }
                // Wait for lock to be released without generating cache misses
                while (_lock.lock.load(std::memory_order_relaxed)) {
                    // Issue X86 PAUSE or ARM YIELD instruction to reduce contention between
                    // hyper-threads
                    __builtin_ia32_pause();
                }
            }
        }

        ~Spinlock() noexcept {
            _lock.lock.store(false, std::memory_order_release);
        }
    };
}
