cmake_minimum_required(VERSION 3.10.0)

project(qs.hpp)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR}/3rdparty/htest.hpp)

add_subdirectory(examples)
add_subdirectory(tests)
