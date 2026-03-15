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
#include "Core/Logger/Logger.hpp"

#include <exception>

namespace Graphite::Core::Renderer::Backends::Vulkan {

VulkanRenderer::VulkanRenderer()
{
    LOG_SCOPE("");
    LOG_CRITICAL(">> VulkanRenderer constructed");
    LOG_CRITICAL(">> Not supported yet, terminating the program");
    std::terminate();
}

VulkanRenderer::~VulkanRenderer()
{
    Cleanup();
}

void VulkanRenderer::Init(Graphite::Core::Application::WindowConfiguration const& window_configuration)
{
}

void VulkanRenderer::Render(std::shared_ptr<IRenderable> user_interface)
{
}

void VulkanRenderer::Cleanup()
{
}

} // namespace Graphite::Core::Renderer::Backends::Vulkan
