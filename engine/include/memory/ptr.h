#pragma once

#include "memory/arc.h"

namespace nebula::arc {
    template<typename T>
    class Ptr {
        bool _isArray{false};
        T* _ptr{};

    public:
        Ptr() = default;

        template<typename ...Args>
        explicit Ptr(Args ...args) {
            _ptr = new T(std::forward<Args>(args)...);

            arc::ARC::get()->update(_ptr, 1);
        }

        explicit Ptr(size_t arrSize) {
            _ptr = new T[arrSize];
            arc::ARC::get()->update(_ptr, 1);

            _isArray = true;
        }

        ~Ptr() {
            if (_ptr != nullptr) {
                if (arc::ARC::get()->update(_ptr, -1)) {
                    if (_isArray) {
                        delete[] _ptr;
                        return;
                    }

                    delete _ptr;
                }
            }
        }

        Ptr(const Ptr<T>& right) {
            _ptr = right.get();
            arc::ARC::get()->update(_ptr, 1);
        }

        Ptr(Ptr<T>&& right)  noexcept {
            _ptr = right.get();
            right._ptr = nullptr;
        }

        Ptr<T>& operator=(const Ptr<T>& right) {
            if (&right == this)
                return *this;

            _ptr = right.get();
            arc::ARC::get()->update(_ptr, 1);

            return *this;
        }

        Ptr<T>& operator=(Ptr<T>&& right) noexcept {
            _ptr = right.get();
            right._ptr = nullptr;

            return *this;
        }

        T& operator*() const {
            return *_ptr;
        }

        auto get() const {
            return _ptr;
        }
    };
}

namespace nebula {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    Scope<T> Scoped(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    Ref<T> Reference(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
