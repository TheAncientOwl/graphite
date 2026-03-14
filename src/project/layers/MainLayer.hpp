/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file MainLayer.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Main Layer.
///

#pragma once

#include "core/application/ApplicationLayer.hpp"
#include "project/AppState.hpp"

namespace Graphite::Project::Layers {

class MainLayer : public Graphite::Core::Application::ILayer<AppState>
{
public:
    static std::string_view GetLayerName();

    MainLayer();

    std::string_view GetName() override;
    void OnPush(AppState& app_state) override;
    void OnPop(AppState& app_state) override;
    void OnBeforeRender(AppState& app_state) override;
    void OnRender(AppState& app_state) override;
    void OnAfterRender(AppState& app_state) override;
    void OnShutdown(AppState& app_state) override;

private:
    void RenderPlayersSelect(AppState& app_state);
    void RenderPlayersTable(AppState& app_state);
    void RenderEditSelectedPlayer(AppState& app_state);
    void RenderMenuBar(AppState& app_state);
    void RenderSearchFilter(AppState& app_state);

private:
    void SavePlayers(AppState& app_state);
    void CleanupBanned(AppState& app_state);
    void SetEditPlayer(AppState& app_state, std::size_t const index);
    bool NameMatchesSearch(AppState& app_state, std::string_view const name) const;
    bool IsRenderable(AppState& app_state, Player const& player) const;
};

} // namespace Graphite::Project::Layers
