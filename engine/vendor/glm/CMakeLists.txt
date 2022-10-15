cmake_minimum_required(VERSION 3.14)

set(lib_name glm)
set(target_name ext_${lib_name})

project(${lib_name})

add_subdirectory(lib)

add_library(${target_name} ALIAS glm)
