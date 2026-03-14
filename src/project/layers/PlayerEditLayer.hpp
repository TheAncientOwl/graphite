/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file PlayerEditLayer.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Edit Player data layer.
///

#pragma once

#include "core/application/ILayer.hpp"
#include "project/AppState.hpp"
#include "project/PlayersApplication.hpp"

namespace Graphite::Project::Layers {

class PlayerEditLayer : public Graphite::Core::Application::ILayer<AppState>
{
public:
    static std::string_view GetLayerName() noexcept;
    std::string_view GetName() const noexcept override;

    PlayerEditLayer(Graphite::Project::PlayersApplication::Ptr application);

    void OnPush() override;
    void OnRender() override;
    void OnPop() override;

private:
    void RenderEditSelectedPlayer();
};

} // namespace Graphite::Project::Layers
