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
class TGraphiteApplication;

template <typename ApplicationState>
class ILayer
{
public:
    using Ptr = std::unique_ptr<ILayer<ApplicationState>>;

    ILayer(Graphite::Core::Application::TGraphiteApplication<ApplicationState>::Ptr application)
        : m_application{std::move(application)} {};
    virtual ~ILayer() = default;

private:
    friend class Graphite::Core::Application::TGraphiteApplication<ApplicationState>;

    virtual std::string_view GetName() const noexcept = 0;

    virtual void OnPush() = 0;
    virtual void OnPop() = 0;
    virtual void OnShutdown() = 0;

    virtual void OnBeforeRender() = 0;
    virtual void OnRender() = 0;
    virtual void OnAfterRender() = 0;

protected:
    std::shared_ptr<TGraphiteApplication<ApplicationState>> m_application{nullptr};
};

} // namespace Graphite::Core::Application
