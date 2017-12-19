cmake_minimum_required(VERSION 3.8)
project(Conveyor)

set(CMAKE_CXX_STANDARD 17)

include_directories(../include)

add_executable(ConveyorTests
        main.cpp
        RealTimeConveyor.cpp
        StaticConveyor.cpp)

target_link_libraries(ConveyorTests
        Conveyor
        gtest
)