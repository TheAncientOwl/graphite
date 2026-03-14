set(LIB_UTILS_SOURCES
    UniqueID.cpp
)

add_library(graphite_core_utils STATIC ${LIB_UTILS_SOURCES})

target_include_directories(graphite_core_utils PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core/utils
)
