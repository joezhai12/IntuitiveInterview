cmake_minimum_required(VERSION 3.16)

project(joe_zhai_interview)

set(CMAKE_CXX_STANDARD 11)

if(RUN_UNIT_TESTS)
    enable_testing()
endif()

# modules
add_subdirectory(fan_control)
add_subdirectory(test_subsystem)