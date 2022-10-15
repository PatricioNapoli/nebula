cmake_minimum_required(VERSION 3.14)

set(lib_name pcg-cpp)
set(target_name ext_${lib_name})

project(${lib_name})

target_include_directories(${target_name} PUBLIC lib/include)
