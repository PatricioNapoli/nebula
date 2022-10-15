#pragma once

#include <utility>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cstring>

#include "templating/generic_map_registry.h"
#include "memory/containers.h"
#include "injector/injectable.h"
#include "time/fixed_timer.h"
#include "concurrency/messages.h"
#include "logging/logger.h"

#include "templating/type_hash.h"

namespace nebula {
    class Messaging : public Injectable {
    public:
        Messaging() = default;

        template<typename T>
        void subscribe(std::function<void(T)>&& messageHandler) {
            static_assert(std::is_base_of<messaging::Message, T>::value, "T must inherit from messaging::Message");

            _genericMapReg<hmap<std::uint64_t, vector<std::function<void(T)>>>>()[TypeHash<T>::value()].push_back(messageHandler);
        }

        template<typename T>
        void subscribe(std::function<void(T)>& messageHandler) {
            static_assert(std::is_base_of<messaging::Message, T>::value, "T must inherit from messaging::Message");

            _genericMapReg<hmap<std::uint64_t, vector<std::function<void(T)>>>>()[TypeHash<T>::value()].push_back(messageHandler);
        }

        template<typename T>
        void publish(const T& msg) {
            static_assert(std::is_base_of<messaging::Message, T>::value, "T must inherit from messaging::Message");

            NEBULA_PROFILE;

            for (auto& sub : _genericMapReg<hmap<std::uint64_t, vector<std::function<void(T)>>>>()[TypeHash<T>::value()]) {
                sub(msg);
            }
        }

        void update();
    private:
        Logger* _logger{nullptr};
        FixedTimer* _fixedTimer{nullptr};

        GEN_MAP_REG

    NEBULA_INJECTABLE(Messaging);
    };
}
