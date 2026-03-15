cmake_minimum_required(VERSION 3.25)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(APPLE)
    project(Graphite LANGUAGES C CXX OBJC OBJCXX)

    find_program(CLANG_PATH clang)
    find_program(CLANGXX_PATH clang++)

    if(CLANG_PATH AND CLANGXX_PATH)
        set(CMAKE_C_COMPILER ${CLANG_PATH} CACHE FILEPATH "Use Apple Clang for C" FORCE)
        set(CMAKE_CXX_COMPILER ${CLANGXX_PATH} CACHE FILEPATH "Use Apple Clang for C++" FORCE)
    endif()
else()
    project(Graphite LANGUAGES C CXX)
endif()

# C++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src/Core)
add_subdirectory(src/demo)
add_subdirectory(src/project)

# Create executable
set(SOURCES
    src/main.cpp
)

add_executable(graphite ${SOURCES})

set_target_properties(graphite PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

target_link_libraries(graphite PRIVATE
    graphite_core_logger
    graphite_core_renderer
    graphite_core_application
    graphite_demo
    graphite_project
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
