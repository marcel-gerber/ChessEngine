cmake_minimum_required(VERSION 3.27)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(ChessEngine src/main.cpp)
