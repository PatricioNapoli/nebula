#include "concurrency/thread_pool.h"

#include <thread>

namespace nebula {
    void ThreadPool::mapDependencies(EnvironmentVars& globalEnv) {
        mapFactory<Logger, Logger::Config>(&_logger, Logger::Config(__FILE__));
        map<Random>(&_random);
        mapFactory<FixedTimer, double>(&_fixedTimer, 1.0 / 30.0);
    }

    void ThreadPool::onConstruct() {
        // Threads can return 0 or 1
        auto threads = std::thread::hardware_concurrency();
        if (threads <= 1) {
            threads = 2;
        }

        for (unsigned int i = 0; i < threads - 1; i++){
            auto name = string("nebula.worker." + std::to_string(i));
            _workers.emplace_back(name, false);
        }

        for (auto& w : _workers) {
            w.start();
        }
    }

    void ThreadPool::submit(Ref<Briefing>& briefing) {
        // Create dynamic if all worker threads (dyn or not) are working to avoid blocking
        auto wrker = getFreeWorker();

        _pending.push_back(briefing);

        if (wrker == nullptr) {
            auto name = "nebula.dynamic." + _random->randomAlphaString(8);
            _dynamicWorkers.emplace_back(name, false);
            auto& dw = _dynamicWorkers.back();
            dw.state.briefings.push(briefing);
            dw.start();
        }
        else {
            wrker->state.briefings.push(briefing);
        }
    }

    void ThreadPool::submit(vector<Ref<Briefing>>& chain) {
        if (chain.size() == 1) {
            submit(chain.back());
            return;
        }

        auto m = chain.back();
        chain.pop_back();
        m->addChain(chain);

        for (auto b : chain) {
            submit(b);
        }
        submit(m);
    }

    void ThreadPool::update() {
        NEBULA_PROFILE;

        if (!_fixedTimer->shouldUpdate()) {
            return;
        }

        auto i = std::begin(_pending);
        while (i != std::end(_pending)) {
            if ((*i)->completed) {
                _pending.erase(i);
            }
            ++i;
        }

        _fixedTimer->update();
    }

    bool ThreadPool::isWorking() {
        for (auto& worker : _workers) {
            if (!worker.state.briefings.empty())
                return true;
        }
        for (auto& worker : _dynamicWorkers) {
            if (!worker.state.briefings.empty())
                return true;
        }
        return false;
    }

    Worker* ThreadPool::getFreeWorker() {
        for (auto& worker : _workers) {
            if (!worker.state.working)
                return &worker;
        }
        return nullptr;
    }

    ThreadPool::~ThreadPool() {
        // Wait for threads to finish working
        while (true) {
            if (!isWorking())
                break;
        }

        for (auto& worker : _workers) {
            worker.state.terminate = true;
            worker.thread.join();
        }

        for (auto& worker : _dynamicWorkers) {
            worker.state.terminate = true;
            worker.thread.join();
        }
    }
}
