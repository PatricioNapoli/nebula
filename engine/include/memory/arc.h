#pragma once

#include <hopscotch_map.h>
#include "memory/spinlock.h"

namespace nebula::arc {
    class ARC {
        Spinlock::Lock _lock;

        hmap<void*, unsigned int> _counters;

    public:
        static ARC* get() {
            static auto instance = ARC{};
            return &instance;
        }

    private:
        bool update(void* ptr, unsigned int count) {
            NEBULA_PROFILE;

            if (ptr == nullptr)
                return false;

            Spinlock l(_lock);

            if (!_counters.contains(ptr)) {
                _counters.emplace(ptr, 1);
                return false;
            }

            auto& counter = _counters.at(ptr);
            if (count == -1 && counter == 1) {
                _counters.erase(ptr);
                return ptr != nullptr;
            }

            counter += count;
            return false;
        }

        template<typename T> friend class Ptr;
    };
}
