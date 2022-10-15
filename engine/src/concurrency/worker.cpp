#include "concurrency/worker.h"

#include <functional>
#include <chrono>

#include "time/stopwatch.h"

#include "concurrency/messaging.h"

namespace nebula {
    void callback(std::any& res, Ref<Briefing>& b, WorkerState* state) {
        b->callback.workResult = res;

        if (b->callbackOnMain) {
            state->callbacksOnMain.push(&b->callback);
        }
        else {
            b->callback.fn(res);
            b->completed = true;
        }
    }

    void work(WorkerState* state) {
        Stopwatch sw;

        while (true) {
            if (state->terminate)
                break;

            if (state->briefings.empty()) {
                // If dynamic and waiting for more than 5 seconds, exit
                if (state->dynamic && sw.getMillis() > 5000)
                    break;

                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                continue;
            }

            auto& q = state->briefings;

            if (q.empty())
                continue;

            Ref < Briefing > b = q.front();
            q.pop();

            b->running = state->working = true;

            NEBULA_PROFILE_T(string_utils::format("Work & Chain: %s", b->identifier), state->name);

            if (!b->chain.empty()) {
                auto chainSize = b->chain.size();
                auto doneCount = 0;
                while(doneCount != chainSize) {
                    doneCount = 0;
                    for (auto& chain : b->chain) {
                        if (!chain->running)
                            doneCount += 1;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(25));
                }
            }

            NEBULA_PROFILE_T(string_utils::format("Work: %s", b->identifier), state->name);

            auto res = b->work();

            if (b->callback.fn != nullptr) {
                callback(res, b, state);
            }

            b->running = state->working = false;

            sw.reset();
        }
    }
}
