# Source files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB UTILS_SRC_FILES ${PROJECT_SOURCE_DIR}/src/utils/*.cpp)

# Header files
file(GLOB HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.hpp)
file(GLOB UTILS_HEADER_FILES ${PROJECT_SOURCE_DIR}/src/utils/*.hpp)

# Add all files as Library. Necessary for tests
add_library(ChessLibrary STATIC ${UTILS_SRC_FILES} ${UTILS_HEADER_FILES} ${SRC_FILES} ${HEADER_FILES})

add_executable(ChessEngine main.cpp)

target_link_libraries(ChessEngine ChessLibrary)