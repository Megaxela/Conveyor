cmake_minimum_required(VERSION 3.8)
project(Conveyor)

option(CONVEYOR_BUILD_TESTS "Build tests" On)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

if (${CONVEYOR_BUILD_TESTS})
    add_subdirectory(tests)
endif()

add_library(Conveyor STATIC
    include/Conveyor/Belt.hpp
    include/Conveyor/FuncOperator.hpp
    include/Conveyor/Operator.hpp
    include/Conveyor/StaticBelt.hpp

    src/Conveyor/Belt.cpp
    src/Conveyor/FuncOperator.cpp
        )