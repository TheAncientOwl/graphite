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

#include "Core/Application/ILayer.hpp"
#include "project/AppState.hpp"
#include "project/PlayersApplication.hpp"

namespace Graphite::Project::Layers {

class MainLayer : public Graphite::Core::Application::ILayer<AppState>
{
public:
    static std::string_view GetLayerName() noexcept;
    std::string_view GetName() const noexcept override;

    MainLayer(Graphite::Project::PlayersApplication::Ptr application);

    void OnPush() override;
    void OnRender() override;
    void OnPop() override;

private:
    void RenderPlayersSelect();
    void RenderPlayersTable();
    void RenderMenuBar();
    void RenderSearchFilter();

private:
    void SavePlayers();
    void CleanupBanned();
    void SetEditPlayer(std::size_t const index);
    bool NameMatchesSearch(std::string_view const name) const;
    bool IsRenderable(Player const& player) const;
};

} // namespace Graphite::Project::Layers
