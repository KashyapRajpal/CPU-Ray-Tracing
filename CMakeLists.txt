cmake_minimum_required(VERSION 3.10)
project(Raytracing)

# Add source files from the "System" directory
# add_subdirectory(Raytracing/System)

# Add source files from the "Core" directory
add_subdirectory(Raytracing/Core)

set(CMAKE_CXX_STANDARD 17) # Or your desired C++ standard

# Add executable
add_executable(raytracing Raytracing/main.cpp)