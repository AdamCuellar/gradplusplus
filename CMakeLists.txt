cmake_minimum_required(VERSION 3.24)
project(grad__)

set(CMAKE_CXX_STANDARD 14)

file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(grad__ main.cpp ${sources})