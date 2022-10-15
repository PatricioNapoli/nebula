#pragma once

#include <utility>

#include "memory/ptr.h"

#include "ecs/ecs.h"
#include "ecs/component.h"
#include "templating/type_hash.h"

namespace nebula {
    class Scene {
        string _name;

    public:
        Scene() = default;

        explicit Scene(string name) : _name(std::move(name)) {}

        string& name() {
            return _name;
        }

        template <typename T>
        void addComponent(Component* comp) {
            NEBULA_PROFILE;

            auto& compVec = _components[TypeHash<T>::value()];
            compVec.push_back(comp);
        }

        template <typename T>
        vector<T*> getComponent() {
            NEBULA_PROFILE;

            auto compVec = vector<T*>();

            if (_components.count(TypeHash<T>::value()) > 0) {
                compVec.reserve(_components[TypeHash<T>::value()].size());

                for (auto& c : _components[TypeHash<T>::value()]) {
                    compVec.push_back(c->template impl<T>());
                }
            }

            return compVec;
        }

        auto& components() {
            return _components;
        }

        hmap<std::uint64_t, vector<Component*>> _components{};
    };
}
