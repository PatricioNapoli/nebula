#pragma once

#include "time/time_types.h"

#include <thread>
#include <utility>

#include "memory/containers.h"

namespace nebula {

    class Stopwatch {
        HRChrono::time_point _start;

    public:
        Stopwatch() {
            reset();
        }

        void reset() {
            _start = HRChrono::now();
        }

        time_t getStart() {
            return std::chrono::duration_cast<Microseconds>(_start.time_since_epoch()).count();
        }

        long getMillis() {
            return std::chrono::duration_cast<Milliseconds>(HRChrono::now() - _start).count();
        }

        long getMicros() {
            return std::chrono::duration_cast<Microseconds>(HRChrono::now() - _start).count();
        }

        long getNanos() {
            return std::chrono::duration_cast<Nanoseconds>(HRChrono::now() - _start).count();
        }
    };

    struct ProfilingResult {
        string data{};
        string name{};
        string thread{};
        time_t start{};
        long nanos{0};
        long micros{0};
        long millis{0};

        ProfilingResult() = default;

        ProfilingResult(string data, string name, string thread, time_t start, long nanos, long micros, long millis)
        : data(std::move(data)), name(std::move(name)), thread(std::move(thread)), start(start), nanos(nanos), micros(micros), millis(millis) {}
    };

    template<typename Fn>
    class StopwatchProfiler : public Stopwatch {
        string _data;
        string _name;
        string _thread;
        Fn _callback;

    public:
        StopwatchProfiler(string data, string name, string thread, Fn &&callback) :
                _data(std::move(data)),
                _name(std::move(name)),
                _thread(std::move(thread)),
                _callback(callback) {
            reset();
        }

        StopwatchProfiler(string name, string thread, Fn &&callback) :
                _name(std::move(name)),
                _thread(std::move(thread)),
                _callback(callback) {
            reset();
        }

        ~StopwatchProfiler() {
            _callback(ProfilingResult(_data, _name, _thread, getStart(), getNanos(), getMicros(), getMillis()));
        }
    };
}
