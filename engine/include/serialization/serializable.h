#pragma once

#include <cstdlib>
#include <ostream>
#include <vector>

#include "profiling/profiler.h"
#include "serialization/serialized_object.h"

#define NEBULA_SERIALIZABLE(type, ext, version, ...) \
    public: \
        static constexpr const char* __EXT = #ext; \
        static constexpr unsigned int __VER = version; \
    protected: \
        void serialize() { registerSerializeRec(__VA_ARGS__); } \
        void deserialize() { registerDeserializeRec(__VA_ARGS__); } \

namespace nebula {

    template <typename T>
    class Serializable {

    template <typename F> friend class Persistence;
    template <typename F> friend class SerializedObject;

    public:
        Serializable() = default;

        virtual ~Serializable() = default;

        SerializedObject<T>& serializeObj() {
            NEBULA_PROFILE;

            _serial.clear();
            _serial.addId();
            serialize();

            return _serial;
        }

        SerializedObject<T>& deserializeObj() {
            NEBULA_PROFILE;

            _serial.removeId();
            deserialize();

            return _serial;
        }

    protected:
        virtual void serialize() = 0;
        virtual void deserialize() = 0;

        template <typename F, typename...Fs>
        void registerSerializeRec(F* first, Fs*... rest) {
            registerSerialize(first);
            registerSerializeRec(rest...);
        }

        template <typename F, typename...Fs>
        void registerDeserializeRec(F* first, Fs*... rest) {
            registerDeserialize(first);
            registerDeserializeRec(rest...);
        }

        void registerSerializeRec() {}
        void registerDeserializeRec() {}

        SerializedObject<T> _serial{};

    private:
        template <typename F>
        void registerSerialize(F* t) {
            _serial << *t;
        }

        template <typename F>
        void registerDeserialize(F* t) {
            _serial >> *t;
        }
    };
}
