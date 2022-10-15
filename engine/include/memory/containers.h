#pragma once

#include <cstdlib>
#include <memory>
#include <iostream>
#include <queue>

#include "memory/allocators/default_allocator.h"

#include <hopscotch_map.h>

namespace nebula {
    template<typename T>
    using vector = std::vector<T, DefaultAllocator<T>>;

    template<typename T>
    using queue = std::queue<T, std::deque<T, DefaultAllocator<T>>>;

    template<typename Key, typename T>
    using hmap = tsl::hopscotch_map<Key, T, std::hash<Key>, std::equal_to<Key>, DefaultAllocator<std::pair<Key, T>>, 62, false, tsl::hh::power_of_two_growth_policy<2>>;

    typedef std::basic_string<char, std::char_traits<char>, DefaultAllocator<char>> string;
    typedef std::basic_stringstream<char, std::char_traits<char>, DefaultAllocator<char>> stringstream;
    typedef std::basic_ostringstream<char, std::char_traits<char>, DefaultAllocator<char>> ostringstream;
}
