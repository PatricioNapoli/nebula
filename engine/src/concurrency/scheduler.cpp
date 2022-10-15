#include "concurrency/scheduler.h"

#include "profiling/profiler.h"

namespace nebula {
    void Scheduler::mapDependencies(EnvironmentVars& globalEnv) {
        map<ThreadPool>(&_threadPool);
        mapFactory<FixedTimer, double>(&_fixedTimer, 1.0 / 300.0);
    }

    void Scheduler::onConstruct() {}

    void Scheduler::update() {
        NEBULA_PROFILE;

        if (!_fixedTimer->shouldUpdate()) {
            return;
        }

        checkCallbacks(_threadPool->getWorkers());
        checkCallbacks(_threadPool->getDynamicWorkers());

        for (auto& t : _repeatingTasks) {
            auto time = SystemTime{}.getEpoch();
            if (time > t->scheduledAt + t->runAfter) {
                _threadPool->submit(t);
                t->scheduledAt = time;
            }
        }

        auto i = std::begin(_scheduledTasks);
        while (i != std::end(_scheduledTasks)) {
            auto& task = (*i);
            if (SystemTime{}.getEpoch() > task->scheduledAt + task->runAfter) {
                _threadPool->submit(task);
                _scheduledTasks.erase(i);
            }

            ++i;
        }

        _fixedTimer->update();
    }

    void Scheduler::scheduleInterval(Ref<Briefing>& briefing) {
        _repeatingTasks.push_back(briefing);
    }

    void Scheduler::schedule(Ref<Briefing>& briefing) {
        _scheduledTasks.push_back(briefing);
    }

    void Scheduler::removeSchedule(Ref<Briefing>& briefing) {
        if (briefing->type != Briefing::SCHEDULED) {
            return;
        }

        auto i = std::begin(_scheduledTasks);
        while (i != std::end(_scheduledTasks)) {
            auto& task = (*i);
            if (task->identifier == briefing->identifier) {
                _scheduledTasks.erase(i);
            }

            ++i;
        }
    }

    void Scheduler::runAsync(Ref<Briefing>& briefing) {
        _threadPool->submit(briefing);
    }

    void Scheduler::runAsync(vector<Ref<Briefing>>& briefing) {
        _threadPool->submit(briefing);
    }

    void Scheduler::checkCallbacks(vector<Worker>& wrkers) {
        for (Worker& w : wrkers) {
            auto& cbQ = w.state.callbacksOnMain;
            if (!cbQ.empty()) {
                auto cb = cbQ.front();
                cbQ.pop();
                cb->fn(cb->workResult);
                cb->parent->completed = true;
            }
        }
    }
}
