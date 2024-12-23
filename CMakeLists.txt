cmake_minimum_required(VERSION 3.10)
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
target_include_directories(lj_sim PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lj_sim PRIVATE -Wall -Wextra -O3)
endif()
