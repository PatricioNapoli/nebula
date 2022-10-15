cmake_minimum_required(VERSION 3.14)
project(nebula_engine)

if (NEBULA_ASAN)
    add_definitions(-DNEBULA_ASAN=1)
    set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
endif (NEBULA_ASAN)

set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${ASAN_FLAGS} -fvisibility=hidden -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-return-std-move -Wno-format-security -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${ASAN_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "/O3")
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

if (NEBULA_PROFILING)
    add_definitions(-DNEBULA_PROFILING=1)
endif (NEBULA_PROFILING)

if (NEBULA_DEBUG)
    add_definitions(-DNEBULA_DEBUG=1)
endif (NEBULA_DEBUG)

if (NEBULA_OPENGL)
    add_definitions(-DNEBULA_OPENGL=1)
endif (NEBULA_OPENGL)

if (NEBULA_VULKAN)
    add_definitions(-DNEBULA_VULKAN=1)
endif (NEBULA_VULKAN)

FILE(GLOB_RECURSE SRCS src src/*.cpp)
add_library(nebula_engine STATIC ${SRCS})

set_target_properties(nebula_engine PROPERTIES DEBUG_POSTFIX "")

include(vendor/Includes.cmake)
INCLUDE_VENDOR(vendor)

add_subdirectory(vendor)

target_include_directories(nebula_engine
        PUBLIC
        "include"
        "${CMAKE_CURRENT_BINARY_DIR}/vendor/assimp/lib/include"
        )
