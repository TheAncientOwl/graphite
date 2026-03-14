/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file MetalRenderer.hpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Metal implementation of @see core/Renderer.hpp.
/// Adaptation of @see external/imgui/examples/example_glfw_metal/main.mm
///

#pragma once

#include <memory>

#include "core/application/renderer/Renderer.hpp"

namespace Graphite::Core::Renderer::Backends::Metal {

class MetalRenderer : public IRenderer
{
public:
    MetalRenderer();
    ~MetalRenderer() override;

    void Init(Graphite::Core::Application::WindowConfiguration const& window_configuration) override;
    void Render(std::shared_ptr<IRenderable> user_interface) override;
    void Cleanup() override;

private:
    struct State;
    std::unique_ptr<State> m_state{nullptr};
    bool m_cleaned_up{false};
};

} // namespace Graphite::Core::Renderer::Backends::Metal
