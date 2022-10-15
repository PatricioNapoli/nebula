#pragma once

#include "memory/containers.h"

#include <iostream>
#include <utility>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_SIMD_AVX2

#include "config/engine_config.h"
#include "logging/logger.h"
#include "concurrency/thread_pool.h"
#include "window/window.h"
#include "input/input.h"
#include "concurrency/messaging.h"
#include "system/system_info.h"
#include "config/configuration.h"
#include "time/delta_timer.h"
#include "camera/camera.h"
#include "graphics/opengl/opengl.h"

namespace nebula {
    class Injector;

    class Engine {
    public:
        explicit Engine(Config cfg);
        ~Engine();
        void init();

        void update();

        Injector* getInjector();

        template <typename ...Args>
        Scene* addScene(Args&& ...args) {
            _scenes.emplace_back(Scoped<Scene>(std::forward<Args>(args)...));
            return _scenes.back().get();
        }

        template <typename ...Args>
        Entity* addEntity(Args&& ...args) {
            _entities.emplace_back(Scoped<Entity>(std::forward<Args>(args)...));
            return _entities.back().get();
        }

        void setScene(Scene* s);
    private:
        Injector* _injector{nullptr};
        Logger* _logger{nullptr};
        ThreadPool* _threadPool{nullptr};
        Scheduler* _scheduler{nullptr};
        Window* _window{nullptr};
        Input* _input{nullptr};
        Messaging* _messaging{nullptr};
        SystemInfo* _systemInfo{nullptr};
        Configuration* _configuration{nullptr};
        DeltaTimer* _deltaTimer{nullptr};
        Camera* _camera{nullptr};

        OpenGL* _renderer{nullptr};

        Scene* _scene{nullptr};

        Config _cfg;
        EnvironmentVars _environment;

        vector<Scope<Scene>> _scenes;
        vector<Scope<Entity>> _entities;

        string _graphicsApi{};
    };
}
