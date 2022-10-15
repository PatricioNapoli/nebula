#pragma once

#include "injector/injectable.h"
#include "time/fixed_timer.h"
#include "concurrency/worker.h"
#include "concurrency/briefing.h"
#include "concurrency/thread_pool.h"

namespace nebula {
    class Scheduler : public Injectable {
        ThreadPool* _threadPool{nullptr};
        FixedTimer* _fixedTimer{nullptr};

    public:
        Scheduler() = default;

        void update();

        void scheduleInterval(Ref<Briefing>& briefing);
        void schedule(Ref<Briefing>& briefing);
        void removeSchedule(Ref<Briefing>& briefing);

        void runAsync(Ref<Briefing>& briefing);
        void runAsync(vector<Ref<Briefing>>& briefing);

    private:
        void checkCallbacks(vector<Worker>& wrkers);

        vector<Ref<Briefing>> _repeatingTasks{};
        vector<Ref<Briefing>> _scheduledTasks{};

    NEBULA_INJECTABLE(Scheduler);
    };
}
