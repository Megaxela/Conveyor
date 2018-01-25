cmake_minimum_required(VERSION 3.8)
project(Conveyor)

option(CONVEYOR_BUILD_TESTS "Build tests" On)
option(CONVEYOR_BUILD_BENCHMARK "Build benchmarks" On)

set(CMAKE_CXX_STANDARD 17)

if (${CONVEYOR_BUILD_TESTS})
    add_subdirectory(tests)
endif()

if (${CONVEYOR_BUILD_BENCHMARK})
    add_subdirectory(benchmark)
endif()

add_library(Conveyor STATIC
    include/Conveyor/Belt.hpp
    include/Conveyor/FuncOperator.hpp
    include/Conveyor/Operator.hpp
    include/Conveyor/StaticBelt.hpp

    src/Conveyor/Belt.cpp
    src/Conveyor/FuncOperator.cpp
)

target_include_directories(Conveyor PUBLIC
        include
)