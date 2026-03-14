set(LIB_UTILS_SOURCES
    DynamicLibrary.cpp
)

add_library(graphite_core_lib_utils STATIC ${LIB_UTILS_SOURCES})

target_include_directories(graphite_core_lib_utils PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core/lib-utils
)
