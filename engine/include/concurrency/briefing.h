#pragma once

#include <algorithm>
#include <any>
#include <utility>

#include "time/system_time.h"
#include "memory/ptr.h"
#include "random/random.h"
#include "templating/generic_map_registry.h"
#include "string/string_utils.h"

namespace nebula {
    struct Briefing;

    struct CallbackContainer {
        Briefing* parent{nullptr};
        std::function<void(std::any&)> fn{nullptr};
        std::any workResult{nullptr};

        CallbackContainer() = default;
        explicit CallbackContainer(Briefing* parent, std::function<void(std::any&)>&& fn) : parent(parent), fn(std::move(fn)) {}
        explicit CallbackContainer(Briefing* parent, std::function<void(std::any&)>& fn) : parent(parent), fn(fn) {}
    };

    struct Briefing {
        string identifier{};

        enum Type {
            SINGLE,
            REPEATING,
            SCHEDULED
        } type{SINGLE};

        std::function<std::any()> work{nullptr};
        CallbackContainer callback{};

        vector<Ref<Briefing>> chain;

        long scheduledAt{SystemTime{}.getEpoch()};
        long runAfter{0};

        bool callbackOnMain{false};
        bool running{false};
        bool completed{false};

        explicit Briefing(Type type, std::function<std::any()>&& work) : identifier(createId("UNKN")), type(type), work(std::move(work)) {}
        explicit Briefing(Type type, std::function<std::any()>& work) : identifier(createId("UNKN")), type(type), work(work) {}

        explicit Briefing(string id, Type type, std::function<std::any()>&& work) : identifier(createId(std::move(id))), type(type), work(std::move(work)) {}
        explicit Briefing(string id, Type type, std::function<std::any()>& work) : identifier(createId(std::move(id))), type(type), work(work) {}

        Briefing* withCallback(std::function<void(std::any&)>&& cb) {
            callback = CallbackContainer(this, std::move(cb));
            return this;
        }

        Briefing* withCallback(std::function<void(std::any&)>& cb) {
            callback = CallbackContainer(this, cb);
            return this;
        }

        Briefing* addChain(const vector<Ref<Briefing>>& _chain) {
            chain = _chain;
            return this;
        }

        Briefing* onMain() {
            callbackOnMain = true;
            return this;
        }

        Briefing* after(long interval) {
            runAfter = interval;
            return this;
        }

    private:
        string createId(string&& id) {
            return string_utils::format("%s(%s)", id, Random::randomAlphaStringG(8));
        }
    };
}
