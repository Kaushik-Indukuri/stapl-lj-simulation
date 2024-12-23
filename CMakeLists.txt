cmake_minimum_required(VERSION 2.8.12.2)
project(lj_simulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(lj_sim
    src/main.cpp
    src/system.cpp
    src/input.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_definitions(-Wall -Wextra -O3)
endif()
