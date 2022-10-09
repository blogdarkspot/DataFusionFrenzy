add_executable(Benchmark benchmark.cpp)
target_link_libraries(Benchmark benchmark::benchmark conflation::conflation)