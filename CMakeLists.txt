cmake_minimum_required(VERSION 3.27)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

# Add GTest submodule
add_subdirectory(./googletest)

# Source files
add_executable(ChessEngine src/main.cpp)

# Test files
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(ChessEngineTests ${TEST_SRC_FILES})

# Link Tests to GTest main
target_link_libraries(ChessEngineTests GTest::gtest_main)

enable_testing()