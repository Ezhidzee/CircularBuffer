cmake_minimum_required(VERSION 3.24)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 17)

add_executable(CircularBuffer tests.cpp CCircularBuffer.hpp CCircularBufferExt.hpp)

add_subdirectory(lib/googletest-main)
include_directories(lib/googletest-main/googletest/include)
include_directories(lib/googletest-main/googlemock/include)
target_link_libraries(CircularBuffer gtest gtest_main)
target_link_libraries(CircularBuffer gmock)
