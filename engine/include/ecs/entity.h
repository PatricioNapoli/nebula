#pragma once

#include <unordered_map>
#include <utility>

#include "profiling/profiler.h"
#include "scene/scene.h"
#include "templating/type_hash.h"
#include "random/random.h"

namespace nebula {
    class Scene;

    class Entt {
        string _name{};
        Scene* _scene{nullptr};
        Entt* _parent{nullptr};
        hmap<string, Entt*> _children;

    public:
        Entt() : _name(Random::randomAlphaStringG(16)) {}

        explicit Entt(Scene* scene) : _name(Random::randomAlphaStringG(16)), _scene(scene) {}
        Entt(string s, Scene* scene) : _name(std::move(s)), _scene(scene) {}

        string name() const {
            return _name;
        }

        Scene* scene() {
            return _scene;
        }

        Entt* parent() {
            return _parent;
        }

        auto& children() {
            return _children;
        }

        void addParent(Entt* parent) {
            _parent = parent;
        }

        void addChild(Entt* child) {
            _children[child->name()] = child;
        }

        void removeChild(Entt* child) {
            _children.erase(child->name());
        }

        void removeFromParent() {
            if (_parent == nullptr)
                return;

            _parent->removeChild(this);
        }
    };
}
