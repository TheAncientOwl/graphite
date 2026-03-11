/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file GraphiteApplication.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Main application.
///

#include <vector>

#include "core/application/WindowConfiguration.hpp"
#include "core/renderer/Renderer.hpp"

namespace Graphite::Core::Application {

template <typename State>
class GraphiteApplication : public Renderer::IRenderable
{
public:
    GraphiteApplication(WindowConfiguration&& window_configuration, State&& initial_state);

private:
    WindowConfiguration m_window_configuration{};
    State m_app_state{};
};

} // namespace Graphite::Core::Application
