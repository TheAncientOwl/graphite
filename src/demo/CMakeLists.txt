add_library(graphite_demo STATIC
    calculator/CalculatorApp.cpp
    calculator/states/Calculator.cpp
)

target_link_libraries(graphite_demo PRIVATE
    graphite_core_logger
    graphite_core_renderer
)

target_include_directories(graphite_demo PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
