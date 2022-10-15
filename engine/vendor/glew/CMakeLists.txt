cmake_minimum_required(VERSION 3.14)

set(lib_name glew)
set(target_name ext_${lib_name})

project(${lib_name})

add_subdirectory(lib/build/cmake)

find_package(GLEW REQUIRED)

add_library(${target_name} STATIC lib/empty.cpp)
target_link_libraries(${target_name} PUBLIC GLEW::glew_s)
