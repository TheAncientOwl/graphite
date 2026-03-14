add_library(dummy_plugin SHARED
    DummyPlugin.cpp
)

target_include_directories(dummy_plugin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(dummy_plugin PRIVATE graphite_core_logger)

set_target_properties(dummy_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/plugins
)
