cmake_minimum_required(VERSION 3.8)
project(ConveyorBenchmark)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS
        "-ftemplate-depth=1500 -O3"
)

set(BENCHMARK_ENABLE_TESTING Off)

add_subdirectory(benchmark)

add_executable(ConveyorBenchmark
        main.cpp
        bench_extend/BenchmarkingExtend.hpp
        bench_extend/TemplateFunctionBenchmark.hpp
)

target_link_libraries(ConveyorBenchmark
        Conveyor
        gtest
        benchmark
)