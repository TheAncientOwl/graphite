cmake_minimum_required(VERSION 3.25)
project(graphite LANGUAGES C CXX OBJC OBJCXX)

# Platform specific compiler
if(APPLE)
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
endif()

# C++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/core)
add_subdirectory(src/demo)

# Create executable
set(SOURCES
    src/main.cpp
)

add_executable(graphite ${SOURCES})
target_link_libraries(graphite PRIVATE
    graphite_core_logger
    graphite_core_renderer
    graphite_core_application
    graphite_demo
)

# Include directories
target_include_directories(graphite PRIVATE
    external
    src
)

# Copy assets to build folder
add_custom_command(TARGET graphite POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:graphite>/assets
    COMMENT "Copying assets folder to build directory"
)
