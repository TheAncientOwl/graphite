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

# Create executable
set(SOURCES
    src/main.cpp
)

add_executable(graphite ${SOURCES})
target_link_libraries(graphite PRIVATE
    graphite_core_logger
    graphite_core_renderer
)

# Include directories
target_include_directories(graphite PRIVATE
    external
    src
)
