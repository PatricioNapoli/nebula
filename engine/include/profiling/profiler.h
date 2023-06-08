#pragma once

#include "time/stopwatch.h"

#include <ostream>
#include <sstream>
#include <iomanip>

#include "engine_debug.h"

#ifdef NEBULA_PROFILING
#define NEB_TOKENPASTE(x, y) x ## y
#define NEB_TOKENPASTE2(x, y) NEB_TOKENPASTE(x, y)
#define NEBULA_PROFILE nebula::StopwatchProfiler NEB_TOKENPASTE2(profiler, __LINE__)(NEB_FUNC_SIG, "main", [&](auto result){ nebula::Profiler::get()->push(std::move(result)); })
#define NEBULA_PROFILE_D(data) nebula::StopwatchProfiler NEB_TOKENPASTE2(profiler, __LINE__)(data, NEB_FUNC_SIG, "main", [&](auto result){ nebula::Profiler::get()->push(std::move(result)); })
#define NEBULA_PROFILE_T(data, thread) nebula::StopwatchProfiler NEB_TOKENPASTE2(profiler, __LINE__)(data, NEB_FUNC_SIG, thread, [&](auto result){ nebula::Profiler::get()->push(std::move(result)); })
#else
#define NEBULA_PROFILE
#define NEBULA_PROFILE_D(data)
#define NEBULA_PROFILE_T(data, thread)
#endif

namespace nebula {
    class Profiler {

    public:
        static Profiler* get() {
            static auto instance = Profiler{};
            return &instance;
        }

        void push(ProfilingResult&& r) {
            _results.push(std::forward<ProfilingResult>(r));
        }

        static bool constexpr isEnabled() {
#ifdef NEBULA_PROFILING
            return true;
#else
            return false;
#endif
        }

        string jsonResults() {
            ostringstream json;

            json << "{";

            json << R"("traceEvents\":[)";

            int i = 0;
            while (!_results.empty()) {
                ProfilingResult r = _results.front();
                _results.pop();

                json << std::setprecision(3) << std::fixed;
                if (i != 0)
                    json << ",{";
                else
                    json << "{";
                json << R"("cat":"function",)";
                json << R"("dur":)" << (r.micros) << ',';

                if (!r.data.empty())
                    json << R"("name":")" << r.name << " " << r.data << "\",";
                else
                    json << R"("name":")" << r.name << "\",";

                json << R"("ph":"X",)";
                json << R"("pid":0,)";
                json << R"("tid":")" << r.thread << R"(",)";
                json << R"("ts":)" << r.start;
                json << "}";

                i++;
            }

            json << "]}";

            return json.str();
        }

        auto& results() {
            return _results;
        }

    private:
        queue<ProfilingResult> _results{};
    };
}
