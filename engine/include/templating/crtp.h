#pragma once

namespace nebula {
#define NEBULA_CRTP \
    template<typename T> \
    T* impl() { \
        return static_cast<T*>(this); \
    }
}
