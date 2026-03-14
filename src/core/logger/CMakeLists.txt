add_library(graphite_core_logger SHARED
    Logger.cpp
    Logger.hpp
    Ansi.cpp
    Ansi.hpp
    LogFormatter.cpp
    LogFormatter.hpp
)

target_include_directories(graphite_core_logger
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
