#pragma once

#include "memory/containers.h"
#include "injector/injectable.h"

#include "ecs/entity.h"
#include "ecs/component.h"

#include "templating/type_seq.h"

namespace nebula {
    class ECS : public Injectable {

        struct ComponentEntitiesPair {
            Ref<Component> component{};
            vector<Ref<Entt>> entities{};

            ComponentEntitiesPair() = default;
        };

        hmap<string, Ref<Entt>> _entities;
        hmap<std::uint64_t, ComponentEntitiesPair> _components;

    public:
        void pushEntity(Ref<Entt>& entity) {
            _entities[entity->name()] = entity;
        }

        template<typename T>
        void pushComponent(Ref<Component>& component, Ref<Entt>& entt) {
            constexpr auto id = TypeHash<T>::value();

            auto& pair = _components[id];
            pair.component = component;
            pair.entities.push_back(entt);
        }

        template<typename ...Ts>
        vector<Ref<ComponentEntitiesPair>> view() {
            vector<Ref<ComponentEntitiesPair>> vec{};
            [&](...){ }((single<Ts>(vec))...);
            return vec;
        }

    private:
        template<typename T>
        int single(vector<Ref<ComponentEntitiesPair>>& chain) {
            constexpr auto id = TypeHash<T>::value();
            chain.push_back(Reference<ComponentEntitiesPair>(_components[id]));
            return 0;
        }

    NEBULA_INJECTABLE(ECS);
    };
}
