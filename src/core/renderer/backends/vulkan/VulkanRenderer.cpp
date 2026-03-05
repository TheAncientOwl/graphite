/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file VulkanRenderer.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see VulkanRenderer.hpp.
///

#include "VulkanRenderer.hpp"
#include "core/logger/Logger.hpp"

#include <exception>

USE_LOG_TAG(Graphite::Core::Renderer::Backends::Vulkan);

namespace Graphite::Core::Renderer::Backends::Vulkan {

VulkanRenderer::VulkanRenderer()
{
    LOG_CRITICAL(">> VulkanRenderer constructed");
    LOG_CRITICAL(">> Not supported yet, terminating the program");
    std::terminate();
}

VulkanRenderer::~VulkanRenderer()
{
    Cleanup();
}

void VulkanRenderer::Init()
{
}

void VulkanRenderer::Render(std::shared_ptr<IRenderable> user_interface)
{
}

void VulkanRenderer::Cleanup()
{
}

} // namespace Graphite::Core::Renderer::Backends::Vulkan
