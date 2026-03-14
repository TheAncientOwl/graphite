/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file ILayer.hpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief App layer.
///

#pragma once

#include <memory>
#include <string_view>

namespace Graphite::Core::Application {

template <typename ApplicationState>
class ILayer
{
public:
    using Ptr = std::unique_ptr<ILayer<ApplicationState>>;

public:
    virtual std::string_view GetName() const noexcept = 0;
    virtual ~ILayer() = default;

    virtual void OnPush(ApplicationState& app_state) = 0;
    virtual void OnPop(ApplicationState& app_state) = 0;
    virtual void OnShutdown(ApplicationState& app_state) = 0;

    virtual void OnBeforeRender(ApplicationState& app_state) = 0;
    virtual void OnRender(ApplicationState& app_state) = 0;
    virtual void OnAfterRender(ApplicationState& app_state) = 0;
};

} // namespace Graphite::Core::Application
