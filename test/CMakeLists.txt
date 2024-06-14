# Enable testing for ctest
enable_testing()

# Add GTest submodule
add_subdirectory(${PROJECT_SOURCE_DIR}/googletest ${CMAKE_BINARY_DIR}/googletest)

# Test files
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
add_executable(ChessEngineTests ${TEST_SRC_FILES})

# Link Tests to GTest main
target_link_libraries(ChessEngineTests GTest::gtest_main ChessLibrary)

# Adding Tests
add_test(NAME ChessEngineTests COMMAND ChessEngineTests)