#include "engine.h"

#include "memory/arc.h"

#include <utility>
#include <csignal>

#include "system/system_info.h"
#include "injector/injector.h"
#include "logging/sigsegv_handler.h"
#include "input/input.h"
#include "engine_events.h"
#include "profiling/profiler.h"
#include "scene/entity.h"
#include "concurrency/scheduler.h"
#include "monad/result.h"

namespace nebula {
    Engine::Engine(Config cfg) : _cfg(std::move(cfg)) {
        _environment["config"] = &_cfg;

        arc::ARC::get();
    }

    Engine::~Engine() {
        _logger->info("Shutting down Nebula..");

        _logger->info("Gracefully cleaning up thread pool..");

        // Delete thread pool first to prevent injector from deleting thread's dependencies.
        delete _threadPool;
        _logger->info("Destroyed thread pool.");

        if (Profiler::isEnabled()) {
            _logger->info("Saving profiling results..");

            auto r = Profiler::get()->jsonResults();
            _injector->getService<FileManager>()->write("profiling.json", r);
        }

        _logger->info("Session end. Cleaning injector.. Bye bye.");
        delete _injector;
    }

    struct ResTest {
        int a = 0;

        ResTest() = default;
        explicit ResTest(int a) : a(a) {}
    };
    
    void Engine::init() {
        NEBULA_PROFILE;

        signal(SIGSEGV, sigsegvHandler);

        _injector = new Injector(_environment);

        _logger = _injector->makeService<Logger, Logger::Config>(Logger::Config("engine"));

        _logger->info("Initializing engine.");

        _threadPool = _injector->getService<ThreadPool>();
        _scheduler = _injector->getService<Scheduler>();
        _window = _injector->getService<Window>();
        _input = _injector->getService<Input>();
        _messaging = _injector->getService<Messaging>();
        _systemInfo = _injector->getService<SystemInfo>();
        _deltaTimer = _injector->getService<DeltaTimer>();
        _camera = _injector->getService<Camera>();

        _renderer = _injector->getService<OpenGL>();

        _configuration = _injector->getService<Configuration>();

        _graphicsApi = _configuration->getOrDefault<std::string>("graphics_api", "opengl");

        auto b = Reference<Briefing>("Usage Stats", Briefing::REPEATING, [=]() {
            auto sys = _systemInfo->getSystemInfo();
            auto delta = _deltaTimer->getFrameData();
            _logger->info("Usage Stats - Alloc: {}KB - RAM: {}MB - CPU - {:.3g}% - FPS - {} - {}ms", metrics::Memory.CurrentUsage() / 1000, sys.ramUsage / 1000,
                          sys.cpuUsage, delta.fps, delta.ms);

            return nullptr;
        });

        b->after(5000);
        _scheduler->scheduleInterval(b);

        auto sys = _systemInfo->getSystemInfo();
        _logger->info("System Info: CPU - {} - RAM - {}GB - GPU - {} - openGl - {}", sys.cpuModel, sys.totalRam / 1000000000, sys.gpuModel, sys.openGl);
    }

    Injector* Engine::getInjector() {
        return _injector;
    }

    void Engine::setScene(Scene* scene) {
        _scene = scene;
        _messaging->publish(engine::SceneChangedMsg(_scene));
    }

    void Engine::update() {
        while (!_window->isClosed()) {
            NEBULA_PROFILE;

            _deltaTimer->update();
            _messaging->update();
            _scheduler->update();

            _camera->update();

            _renderer->update();

            if (_cfg.mode == config::Standalone)
                _window->update();
        }
    }
}
