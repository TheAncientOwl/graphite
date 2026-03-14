add_library(graphite_project SHARED
    PlayersApplication.cpp
     layers/MainLayer.cpp
)

target_link_libraries(graphite_project PRIVATE
    graphite_core_logger
    graphite_core_renderer
    graphite_core_application
)

target_include_directories(graphite_project PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
