#pragma once

#include "memory/containers.h"
#include "memory/spinlock.h"

namespace nebula {
    template <typename T>
    class SafeQueue {
    public:
        SafeQueue() = default;

        unsigned long size() {
            return _queue.size();
        }

        void push(T element) {
            Spinlock l(_lock);
            _queue.push(element);
        }

        bool empty() {
            return _queue.empty();
        }

        T pop() {
            if (_queue.empty())
                return nullptr;

            Spinlock l(_lock);
            T element = _queue.front();
            _queue.pop();

            return element;
        }

        queue<T>* get() {
            return &_queue;
        }

    private:
        Spinlock::Lock _lock;

        queue<T> _queue{};
    };
}
