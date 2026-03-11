/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file GraphiteApplication.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see GraphiteApplication.hpp.
///

#include "core/application/GraphiteApplication.hpp"

namespace Graphite::Core::Application {

template <typename State>
GraphiteApplication<State>::GraphiteApplication(WindowConfiguration&& window_configuration, State&& initial_state)
    : m_window_configuration{std::move(window_configuration)}, m_app_state{std::move(initial_state)}
{
}

}; // namespace Graphite::Core::Application
