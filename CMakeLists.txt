CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(DataFusionFrenzy)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    add_subdirectory(tests)
endif()

option(PACKAGE_BENCHMARK "Run benchmark" OFF)
if(PACKAGE_BENCHMARK)
    add_subdirectory(benchmark)
endif()
