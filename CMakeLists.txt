CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

project(DataFusionFrenzy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(include)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

option(PACKAGE_BENCHMARK "Run benchmark" ON)
if(PACKAGE_BENCHMARK)
    add_subdirectory(submodules)
    add_subdirectory(benchmark)
endif()