#pragma once

#include "profiling/profiler.h"
#include "serialization/serializable.h"
#include "vector/vector_utils.h"

namespace nebula {

    template <typename T>
    class Persistence {
        Serializable<T>& _obj;
        FileManager* _fm;
        string _path;

    public:
        Persistence(Serializable<T>& obj, FileManager* fm, string path) : _obj(obj), _fm(fm), _path(std::move(path)) {
            if (_path.find(T::__EXT) == string::npos) {
                _path += T::__EXT;
            }
        }

        ~Persistence() {
            _obj._serial.clear();
        }

        void persist() {
            NEBULA_PROFILE_D(_path);

            _obj.serializeObj();
            _fm->write(_path, _obj._serial.body());
        }

        void load() {
            NEBULA_PROFILE_D(_path);

            auto f = _fm->read(_path, FileManager::File::Vector);

            vector_utils::move(f.bytes, _obj._serial.body());
            _obj.deserializeObj();
        }
    };
}
