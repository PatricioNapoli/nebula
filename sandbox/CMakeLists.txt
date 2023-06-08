cmake_minimum_required(VERSION 3.14)
project(nebula_sandbox_cpp)

if (NEBULA_ASAN)
    add_definitions(-DNEBULA_ASAN=1)
    set(ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
endif (NEBULA_ASAN)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${ASAN_FLAGS} -fvisibility=hidden -Wall -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-parameter -Wno-return-std-move -Wno-format-security -Wno-unknown-pragmas")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${ASAN_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "/O3")
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

set(CMAKE_DEBUG_POSTFIX "")

add_subdirectory(engine)

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

if (NEBULA_PROFILING)
    add_definitions(-DNEBULA_PROFILING=1)
endif (NEBULA_PROFILING)

find_library(
        JLOC
        NAMES jemalloc
        PATHS "/usr/local/lib"
)

add_executable(nebula_sandbox main.cpp)

target_include_directories(nebula_sandbox PUBLIC "engine/include")
target_link_libraries(nebula_sandbox PRIVATE nebula_engine nebula_vendor)

if (NEBULA_JEMALLOC)
    add_definitions(-DNEBULA_JEMALLOC=1)
    target_link_libraries(nebula_sandbox PRIVATE ${JLOC})
endif (NEBULA_JEMALLOC)
