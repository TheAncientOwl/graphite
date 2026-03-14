set(LIB_COMMON_SOURCES
    UniqueID.cpp
    DynamicLibrary.cpp
)

add_library(graphite_core_common SHARED ${LIB_COMMON_SOURCES})

target_include_directories(graphite_core_common PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Core/Common
)
