cmake_minimum_required(VERSION 3.14)

set(lib_name hopscotch-map)
set(target_name ext_${lib_name})

project(${lib_name})

add_library(${target_name} STATIC lib/hopscotch_map.cpp)
target_include_directories(${target_name} PUBLIC lib/include)
