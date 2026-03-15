add_library(graphite_demo STATIC
    calculator/CalculatorApp.cpp
    calculator/states/Calculator.cpp
)

set_target_properties(graphite_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

target_link_libraries(graphite_demo PRIVATE
    graphite_core_logger
    graphite_core_renderer
)

target_include_directories(graphite_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

# add_subdirectory(plugin)
