cmake_minimum_required(VERSION 3.14)

set(lib_name xxhash)
set(target_name ext_${lib_name})

project(${lib_name})

add_definitions(-DXXH_INLINE_ALL=ON)

add_subdirectory(lib/cmake_unofficial)

add_library(${target_name} ALIAS xxhash)
