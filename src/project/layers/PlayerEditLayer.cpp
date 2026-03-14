/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file PlayerEditLayer.cpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Implementation of @see PlayerEditLayer.hpp.
///

#include "PlayerEditLayer.hpp"

#include "imgui/imgui.h"

namespace Graphite::Project::Layers {

std::string_view PlayerEditLayer::GetLayerName() noexcept
{
    return "PlayerEditLayer";
}

PlayerEditLayer::PlayerEditLayer(Graphite::Project::PlayersApplication::Ptr application)
    : ILayer{std::move(application)}
{
    LOG_SCOPE("");
};

PlayerEditLayer::PlayerEditLayer(
    Graphite::Project::PlayersApplication::Ptr application,
    Graphite::Core::Utils::UniqueID uid)
    : ILayer{std::move(application), std::move(uid)}
{
}

std::string_view PlayerEditLayer::GetName() const noexcept
{
    return GetLayerName();
}

void PlayerEditLayer::OnPush()
{
    LOG_SCOPE("");
}

void PlayerEditLayer::OnPop()
{
    LOG_SCOPE("");
}

void PlayerEditLayer::OnRender()
{
    RenderEditSelectedPlayer();
}

void PlayerEditLayer::RenderEditSelectedPlayer()
{
    LOG_SCOPE("");
    auto& app_state{m_application->GetApplicationState()};

    if (!app_state.selected_player)
        return;

    auto const player_index{*app_state.selected_player};

    ImGui::Begin("Edit Player");

    if (ImGui::InputText("Edit Name", app_state.edits.buffer, sizeof(app_state.edits.buffer)))
    {
        LOG_INFO(
            "Saving player name -> prev{} -> now{}",
            app_state.players[player_index].name,
            app_state.edits.buffer);
        app_state.players[player_index].name = app_state.edits.buffer;
        app_state.save_players_data = true;
        app_state.reorder_players_data = true;
    }

    ImGui::SliderInt("Health", &app_state.edits.health, 0, 100);
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        LOG_INFO(
            "Saving player health -> prev{} -> now{}",
            app_state.players[player_index].health,
            app_state.edits.health);
        app_state.players[player_index].health = app_state.edits.health;
        app_state.save_players_data = true;
        app_state.reorder_players_data = true;
    }

    if (ImGui::Button("Done"))
    {
        LOG_INFO("Done editing");
        app_state.selected_player = std::nullopt;
        m_application->RemoveLayer(m_layer_uid);
    }

    ImGui::End();
}

} // namespace Graphite::Project::Layers
