cmake_minimum_required(VERSION 3.14)

set(lib_name assimp)
set(target_name ext_${lib_name})

project(${lib_name})

add_definitions(-DBUILD_SHARED_LIBS=OFF)
add_subdirectory(lib)

add_library(${target_name} ALIAS assimp)
