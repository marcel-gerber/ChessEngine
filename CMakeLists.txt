cmake_minimum_required(VERSION 3.27)
project(ChessEngine)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)

# Set Compiler flags
set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Adding /src and /test
add_subdirectory(src)
add_subdirectory(test)