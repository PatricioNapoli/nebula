#pragma once

#include "engine_debug.h"

namespace nebula {
    template<typename Char>
    class TypeHash {
        static constexpr std::uint64_t _offset = 14695981039346656037ull;
        static constexpr std::uint64_t _prime = 1099511628211ull;

    public:
        [[nodiscard]] static constexpr unsigned long long value() noexcept {
            std::string_view func{NEB_FUNC_SIG};

            auto first = func.find_first_not_of(' ', func.find_first_of(NEB_FUNC_SIG_PREFIX) + 1);
            auto value = func.substr(first, func.find_last_of(NEB_FUNC_SIG_SUFFIX) - first);
            auto str = value.data();
            auto size = value.size();

            std::uint64_t partial{_offset};
            while(size--) { partial = (partial ^ (str++)[0]) * _prime; }
            return partial;
        }
    };
}
