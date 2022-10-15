#pragma once

namespace nebula {
    template<typename T, size_t Size>
    class ObjectPool {
    public:

        class PoolObject {
            friend ObjectPool;

            T _obj{};
            size_t _index{};
            bool _isFree{true};

            PoolObject& use() {
                _isFree = false;
                return *this;
            }

            void release() {
                _isFree = true;
            }

        public:
            PoolObject() = default;
            PoolObject(T&& obj, size_t index) : _obj(std::move(obj)), _index(index) {}

            size_t index() const {
                return _index;
            }

            T& obj() {
                return _obj;
            }
        };

        template<typename ...Args>
        explicit ObjectPool(Args ...args) : _objectSize(sizeof(PoolObject)), _size(Size) {
            for (size_t i = 0; i < Size; i++) {
                _free.push(i);
                _objects[i] = PoolObject(T(std::forward<Args>(args)...), i);
            }

            recalcUsage();
        }

        ObjectPool() : _objectSize(sizeof(PoolObject)), _size(Size) {
            for (size_t i = 0; i < Size; i++) {
                _free.push(i);
                _objects[i] = PoolObject(T(), i);
            }

            recalcUsage();
        }

        vector<PoolObject>* getObjects() {
            return &_objects;
        }

        PoolObject& get() {
            NEBULA_ASSERT(!_free.empty(), "object pool ran out of elements, increase pool size");

            size_t index = _free.front();
            _free.pop();

            return _objects[index].use();
        }

        void release(PoolObject& obj) {
            if (obj._isFree) {
                return;
            }

            obj.release();
            _free.push(obj.index());
        }

        size_t freeCount() {
            return _free.size();
        }

        size_t getMemoryUsage() {
            return _memoryUsed;
        }

    private:
        size_t _objectSize{0};

        size_t _size{0};

        std::size_t _memoryUsed{0};

        queue<size_t> _free{};
        PoolObject _objects[Size];

        void recalcUsage() {
            _memoryUsed = _size * _objectSize;
        }
    };
}
