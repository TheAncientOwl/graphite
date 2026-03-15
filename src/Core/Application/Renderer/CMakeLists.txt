# 1. Create Library
set(CORE_RENDERER_SOURCES
    # ImGui
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
    # ${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
)

if(APPLE)
    list(APPEND CORE_RENDERER_SOURCES
        Renderer.mm
        Backends/Metal/MetalRenderer.mm
        Backends/Metal/MetalRendererState.mm
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_metal.mm
    )

    # Set Objective-C++ compiler for Metal backend
    set_source_files_properties(
        Renderer.mm
        Backends/Metal/MetalRenderer.mm
        Backends/Metal/MetalRendererState.mm
        PROPERTIES LANGUAGE OBJCXX
    )
else()
    list(APPEND CORE_RENDERER_SOURCES
        Renderer.cpp
        Backends/Vulkan/VulkanRenderer.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/backends/imgui_impl_vulkan.cpp
        ${PROJECT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
    )
endif()

add_library(graphite_core_renderer SHARED ${CORE_RENDERER_SOURCES})

target_include_directories(graphite_core_renderer PUBLIC
    ${GLFW_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/backends
)

target_link_libraries(graphite_core_renderer PRIVATE graphite_core_logger)

# 2. Set compile definitions
if(APPLE)
    message(STATUS "Graphite::Core::Application::Renderer::GRAPHITE_USE_METAL_API")
    target_compile_definitions(graphite_core_renderer PRIVATE GRAPHITE_USE_METAL_API)

    target_compile_options(graphite_core_renderer PRIVATE
        -Wno-arc-bridge-casts-disallowed-in-nonarc
    )
else()
    message(STATUS "Graphite::Core::Application::Renderer::GRAPHITE_USE_VULKAN_API")
    target_compile_definitions(graphite_core_renderer PRIVATE GRAPHITE_USE_VULKAN_API)
endif()

# 3. Link GLFW
find_package(glfw3 3 REQUIRED)
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(METAL_LIBRARY Metal REQUIRED)
    find_library(METALKIT_LIBRARY MetalKit REQUIRED)
    find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)

    target_link_libraries(graphite_core_renderer PRIVATE
        glfw
        ${COCOA_LIBRARY}
        ${METAL_LIBRARY}
        ${METALKIT_LIBRARY}
        ${QUARTZCORE_LIBRARY}
    )
else()
    find_package(Vulkan REQUIRED)
    target_include_directories(graphite_core_renderer PUBLIC ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(graphite_core_renderer PRIVATE glfw Vulkan::Vulkan)
endif()
