add_library(graphite_project STATIC
    PlayersApplication.cpp
    layers/MainLayer.cpp
    layers/PlayerEditLayer.cpp
)

set_target_properties(graphite_project PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

target_link_libraries(graphite_project PRIVATE
    graphite_core_logger
    graphite_core_renderer
    graphite_core_application
)

target_include_directories(graphite_project PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
