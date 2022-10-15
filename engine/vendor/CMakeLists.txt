cmake_minimum_required(VERSION 3.14)
project(nebula_vendor)

add_library(nebula_vendor empty.cpp)

message("-- Nebula Vendor Libraries Configuration --")

message("-- Adding ASSIMP")
add_subdirectory(assimp)
target_link_libraries(nebula_vendor PUBLIC ext_assimp)

message("-- Adding STB")
add_subdirectory(stb)
target_link_libraries(nebula_vendor PUBLIC ext_stb)

message("-- Adding FMT")
add_subdirectory(fmt)
target_link_libraries(nebula_vendor PUBLIC ext_fmt)

message("-- Adding GLEW")
add_subdirectory(glew)
target_link_libraries(nebula_vendor PUBLIC ext_glew)

message("-- Adding GLFW")
add_subdirectory(glfw)
target_link_libraries(nebula_vendor PUBLIC ext_glfw)

message("-- Adding GLM")
add_subdirectory(glm)
target_link_libraries(nebula_vendor PUBLIC ext_glm)

message("-- Adding YAML-CPP")
add_subdirectory(yaml-cpp)
target_link_libraries(nebula_vendor PUBLIC ext_yaml-cpp)

message("-- Adding SPDLOG")
add_subdirectory(spdlog)
target_link_libraries(nebula_vendor PUBLIC ext_spdlog)

message("-- Adding Sparse-Map")
add_subdirectory(sparse-map)
target_link_libraries(nebula_vendor PUBLIC ext_sparse-map)

message("-- Adding Hopscotch-Map")
add_subdirectory(hopscotch-map)
target_link_libraries(nebula_vendor PUBLIC ext_hopscotch-map)

message("-- Adding Unordered-Dense")
add_subdirectory(unordered-dense)
target_link_libraries(nebula_vendor PUBLIC ext_unordered-dense)

message("-- Adding XXHASH")
add_subdirectory(xxhash)
target_link_libraries(nebula_vendor PUBLIC ext_xxhash)
