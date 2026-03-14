/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file PlayersApplication.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Migration of @see demo/BasicTableApp.
///

#include <filesystem>

#include "AppState.hpp"
#include "core/application/GraphiteApplication.hpp"

namespace Graphite::Project {

class PlayersApplication : public Graphite::Core::Application::GraphiteApplication<AppState>
{
public:
    PlayersApplication(
        Graphite::Core::Application::WindowConfiguration window_configuration,
        AppState initial_state);

    ~PlayersApplication();

    static inline constexpr std::filesystem::path GetPlayersDataPath()
    {
        return std::filesystem::current_path() / "players.txt";
    }

private:
    void LoadPlayers();
    void ApplyTheme();

private:
    void AppInit() override;
};

} // namespace Graphite::Project
