#pragma once

#include "injector/injectable.h"
#include "safe_queue.h"
#include "worker.h"
#include "logging/logger.h"
#include "messaging.h"
#include "random/random.h"
#include "time/fixed_timer.h"

#include <functional>
#include <utility>

namespace nebula {
    class ThreadPool : public Injectable {
    public:
        ThreadPool() = default;
        ~ThreadPool() override;

        void update();

        void submit(vector<Ref<Briefing>>& chain);
        void submit(Ref<Briefing>& briefing);

        vector<Worker>& getWorkers() {
            return _workers;
        }

        vector<Worker>& getDynamicWorkers() {
            return _dynamicWorkers;
        }

    private:
        vector<Worker> _workers{};
        vector<Worker> _dynamicWorkers{};

        vector<Ref<Briefing>> _pending{};

        Logger* _logger{nullptr};
        Random* _random{nullptr};
        FixedTimer* _fixedTimer{nullptr};

        bool isWorking();
        Worker* getFreeWorker();

    NEBULA_INJECTABLE(ThreadPool);
    };
}
