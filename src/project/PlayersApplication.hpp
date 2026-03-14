/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file PlayersApplication.hpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Migration of @see demo/BasicTableApp.
///

#pragma once

#include <filesystem>

#include "AppState.hpp"
#include "TGraphiteApplication.hpp"

namespace Graphite::Project {

class PlayersApplication : public Graphite::Core::Application::TGraphiteApplication<AppState>
{
public:
    using Graphite::Core::Application::TGraphiteApplication<AppState>::Ptr;

    ~PlayersApplication();

    static inline constexpr std::filesystem::path GetPlayersDataPath()
    {
        return std::filesystem::current_path() / "players.txt";
    }

private:
    friend class Graphite::Core::Application::TGraphiteApplication<AppState>;
    PlayersApplication(
        Graphite::Core::Application::WindowConfiguration window_configuration,
        AppState initial_state);

    void LoadPlayers();
    void ApplyTheme();

private:
    void AppInit() override;
};

} // namespace Graphite::Project
