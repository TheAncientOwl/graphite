/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file VulkanRenderer.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Vulkan implementation of @see Core/Renderer.hpp.
///

#pragma once

#include "Core/Renderer/Renderer.hpp"

namespace Graphite::Core::Renderer::Backends::Vulkan {

class VulkanRenderer : public IRenderer
{
public:
    VulkanRenderer();
    ~VulkanRenderer() override;

    void Init() override;
    void Render(std::shared_ptr<IRenderable> user_interface) override;
    void Cleanup() override;
};

} // namespace Graphite::Core::Renderer::Backends::Vulkan
