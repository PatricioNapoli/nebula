#pragma once

#include <thread>

#include "concurrency/briefing.h"
#include "memory/ptr.h"

namespace nebula {
    struct WorkerState {
        queue<Ref<Briefing>> briefings{};
        queue<CallbackContainer*> callbacksOnMain{};

        bool working{false};
        bool terminate{false};

        std::thread::id id;
        string name;
        bool dynamic{false};

        WorkerState(string name, bool dynamic) : name(std::move(name)), dynamic(dynamic) {}
    };

    void work(WorkerState* state);

    struct Worker {
        WorkerState state;
        std::thread thread;

        Worker(string name, bool dynamic) : state(std::move(name), dynamic) {}

        void start() {
            thread = std::thread(work, &state);
            state.id = thread.get_id();
        }
    };
}
