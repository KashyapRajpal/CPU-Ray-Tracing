cmake_minimum_required(VERSION 3.10)
project(Raytracing)

set(CMAKE_CXX_STANDARD 17) # Or your desired C++ standard

file(GLOB_RECURSE CPP_SOURCES "*.cpp")
add_executable(math ${CPP_SOURCES})