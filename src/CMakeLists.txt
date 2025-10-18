cmake_minimum_required(VERSION 3.20)

add_library(Conflation INTERFACE)
target_include_directories(Conflation INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_compile_features(Conflation INTERFACE cxx_std_20)
