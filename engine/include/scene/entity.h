#pragma once

#include <unordered_map>
#include <utility>

#include "profiling/profiler.h"
#include "scene/scene.h"
#include "templating/type_hash.h"

namespace nebula {
    class Scene;

    class Entity {
        string _name{};
        Scene* _scene{nullptr};
        Entity* _parent{nullptr};
        hmap<string, Entity*> _children;

    public:
        Entity() = default;
        Entity(string s, Scene* scene) : _name(std::move(s)), _scene(scene) {}

        string name() const {
            return _name;
        }

        Scene* scene() {
            return _scene;
        }

        Entity* parent() {
            return _parent;
        }

        auto& children() {
            return _children;
        }

        template <typename T, typename ... Args>
        T& addComponent(Args&& ... args) {
            NEBULA_PROFILE;

            _components[TypeHash<T>::value()].push_back(Reference<T>(std::forward<Args>(args)...));

            Component* comp = _components[TypeHash<T>::value()].back().get();
            comp->entity = this;
            _scene->addComponent<T>(comp);
            return *comp->template impl<T>();
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

        hmap<std::uint64_t, vector<Ref<Component>>> _components;
    };
}
