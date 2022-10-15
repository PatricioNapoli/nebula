#pragma once

#include <cstdio>
#include <fmt/format.h>

#include "memory/containers.h"

namespace nebula::string_utils {
    template<typename... Args>
    string format(const char *format, const Args &... args) {
        auto out = fmt::memory_buffer();
        fmt::format_to(std::back_inserter(out), format, args...);
        out.push_back('\0');
        return string(out.data());
    }

    string remove(const string& str, char character);
    void replace(string& data, const string& search, const string& replace);
}
