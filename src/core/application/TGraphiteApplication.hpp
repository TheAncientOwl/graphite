/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file TGraphiteApplication.hpp
/// @author Alexandru Delegeanu
/// @version 0.4
/// @brief Main application.
///

#pragma once

#include <algorithm>
#include <concepts>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "core/logger/Logger.hpp"

#include "ILayer.hpp"
#include "WindowConfiguration.hpp"
#include "renderer/Renderer.hpp"

namespace Graphite::Core::Application {

template <typename ApplicationState>
class TGraphiteApplication
    : public Graphite::Core::Renderer::IRenderable
    , public std::enable_shared_from_this<TGraphiteApplication<ApplicationState>>
{
public:
    TGraphiteApplication(WindowConfiguration window_configuration, ApplicationState initial_state);
    virtual ~TGraphiteApplication() = default;

public:
    void Run();

private:
    virtual void AppInit() = 0;

protected:
    ApplicationState& GetApplicationState();

    template <typename LayerImpl, typename... Args>
        requires std::derived_from<LayerImpl, ILayer<ApplicationState>> && requires {
            { LayerImpl::GetLayerName() } -> std::convertible_to<std::string_view>;
        }
    LayerImpl& PushLayer(Args&&... args);

private:
    void Init();
    void Render() override;
    void Shutdown();

    void RenderLayers();
    void ShutdownLayers();

protected:
    WindowConfiguration m_window_configuration{};
    ApplicationState m_app_state{};

private:
    std::vector<typename ILayer<ApplicationState>::Ptr> m_layers{};
    std::unique_ptr<Graphite::Core::Renderer::IRenderer> m_renderer{nullptr};
};

#pragma region LifeCycle
template <typename ApplicationState>
TGraphiteApplication<ApplicationState>::TGraphiteApplication(
    WindowConfiguration window_configuration,
    ApplicationState initial_state)
    : m_window_configuration{std::move(window_configuration)}, m_app_state{std::move(initial_state)}
{
    LOG_SCOPE("");
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::Run()
{
    LOG_SCOPE("");
    Init();
    m_renderer->Render(this->shared_from_this());
    Shutdown();
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::Init()
{
    LOG_SCOPE("");
    m_renderer = Graphite::Core::Renderer::CreateRenderer();
    m_renderer->Init(m_window_configuration);

    AppInit();
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::Render()
{
    LOG_SCOPE("");
    RenderLayers();
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::Shutdown()
{
    LOG_SCOPE("");
    ShutdownLayers();
    m_renderer->Cleanup();
}
#pragma endregion LifeCycle

#pragma region Internals
template <typename ApplicationState>
template <typename LayerImpl, typename... Args>
    requires std::derived_from<LayerImpl, ILayer<ApplicationState>> && requires {
        { LayerImpl::GetLayerName() } -> std::convertible_to<std::string_view>;
    }
LayerImpl& TGraphiteApplication<ApplicationState>::PushLayer(Args&&... args)
{
    LOG_SCOPE("{}", LayerImpl::GetLayerName().data());
    auto layer = std::make_unique<LayerImpl>(std::forward<Args>(args)...);
    layer->OnPush(m_app_state);
    m_layers.push_back(std::move(layer));
    return static_cast<LayerImpl&>(*m_layers.back());
}

template <typename ApplicationState>
ApplicationState& TGraphiteApplication<ApplicationState>::GetApplicationState()
{
    return m_app_state;
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::RenderLayers()
{
    LOG_SCOPE("");
    std::for_each(m_layers.begin(), m_layers.end(), [this](ILayer<ApplicationState>::Ptr& layer_ptr) {
        layer_ptr->OnBeforeRender(m_app_state);
        layer_ptr->OnRender(m_app_state);
        layer_ptr->OnAfterRender(m_app_state);
    });
}

template <typename ApplicationState>
void TGraphiteApplication<ApplicationState>::ShutdownLayers()
{
    LOG_SCOPE("");
    std::for_each(m_layers.begin(), m_layers.end(), [this](ILayer<ApplicationState>::Ptr& layer_ptr) {
        layer_ptr->OnShutdown(m_app_state);
    });
}
#pragma endregion Internals

} // namespace Graphite::Core::Application
