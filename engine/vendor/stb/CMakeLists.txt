cmake_minimum_required(VERSION 3.14)

set(lib_name stb)
set(target_name ext_${lib_name})

project(${lib_name})

add_library(${target_name} STATIC lib/stb_image.c)
target_include_directories(${target_name} PUBLIC lib/include)
