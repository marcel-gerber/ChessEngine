cmake_minimum_required(VERSION 3.27)
project(ChessEngine)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Compiler flags
set(GCC_COVERAGE_COMPILE_FLAGS "-O3 -march=native -std=c++20")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++ ${GCC_COVERAGE_COMPILE_FLAGS}")

# Activating LTO if supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(NOTICE "INFO: LTO has been disabled, because is not supported in this configuration.")
    message(NOTICE "INFO: This is not critical as it only optimizes performance.")
endif()

# Adding /src and /test
add_subdirectory(src)
add_subdirectory(test)