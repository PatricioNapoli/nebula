#pragma once

namespace nebula {
    class Executor {
        template<auto Fn, typename T, typename ...Args>
        auto exec(T inst, Args... args) {
            return (inst->*Fn)(std::forward<Args>(args)...);
        }

        template<auto Fn, typename T>
        auto execute(T && obj) {
            return [&obj] (auto &&... args) { return (std::forward<T>(obj).*Fn)(std::forward<decltype(args)>(args)...); };
        }
    };
}
