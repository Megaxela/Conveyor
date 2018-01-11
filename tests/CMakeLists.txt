cmake_minimum_required(VERSION 3.8)
project(ConveyorTests)

set(CMAKE_CXX_FLAGS -ftemplate-depth=10240)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(gtest)

include_directories(../include)

add_executable(ConveyorTests
        main.cpp
        RealTimeConveyor.cpp
        StaticConveyor.cpp StaticCopiesCount.cpp TestingExtend.hpp)

target_link_libraries(ConveyorTests
        Conveyor
        gtest
)