cmake_minimum_required(VERSION 3.8)
project(ConveyorBenchmark)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -ftemplate-depth=1500)

include_directories(
        ../include
        benchmark/include
)

set(BENCHMARK_ENABLE_TESTING Off)

add_subdirectory(benchmark)

add_executable(ConveyorBenchmark
        StaticOperatorType.hpp
        main.cpp
)

target_link_libraries(ConveyorBenchmark
        Conveyor
        gtest
        benchmark
)