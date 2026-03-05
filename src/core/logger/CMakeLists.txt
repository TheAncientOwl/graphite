add_library(graphite_core_logger STATIC
    Logger.cpp
    Logger.hpp
)

target_include_directories(graphite_core_logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
