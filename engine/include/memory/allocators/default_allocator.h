#pragma once

#include "memory/metrics.h"

namespace nebula {
    template <typename T>
    class DefaultAllocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;

        template <typename U>
        struct rebind {
            using other = DefaultAllocator<U>;
        };

        DefaultAllocator() = default;
        DefaultAllocator(const DefaultAllocator&) = default;

        template <typename U>
        DefaultAllocator(const DefaultAllocator<U>&) {}

        pointer address(reference x) const noexcept { return &x; }

        const_pointer address(const_reference x) const noexcept { return &x; }

        pointer allocate(size_type n, const void* /*hint*/ = 0) {
            pointer ptr = static_cast<pointer>(std::malloc(n * sizeof(T)));
            if (ptr == nullptr) {
                throw std::bad_alloc();
            }

            nebula::metrics::Memory.usedMemory += n * sizeof(T);
            return ptr;
        }

        void deallocate(T* p, size_type n) {
            nebula::metrics::Memory.usedMemory -= n * sizeof(T);
            free(p);
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max() / sizeof(value_type);
        }

        template <typename U, typename... Args>
        void construct(U* p, Args&&... args) {
            ::new (static_cast<void*>(p)) U(std::forward<Args>(args)...);
        }

        template <typename U>
        void destroy(U* p) {
            p->~U();
        }
    };
}
