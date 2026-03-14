/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file GraphiteApplication.hpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Main application.
///

#include <algorithm>
#include <concepts>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "core/application/ApplicationLayer.hpp"
#include "core/application/WindowConfiguration.hpp"
#include "core/logger/Logger.hpp"
#include "core/renderer/Renderer.hpp"

#include "imgui/imgui.h"

namespace Graphite::Core::Application {

template <typename ApplicationState>
class GraphiteApplication
    : public Graphite::Core::Renderer::IRenderable
    , public std::enable_shared_from_this<GraphiteApplication<ApplicationState>>
{
public:
    GraphiteApplication(WindowConfiguration window_configuration, ApplicationState initial_state);

public:
    ApplicationState& GetApplicationState();

    void Run();

private:
    void Init();
    void Render() override;
    void Shutdown();

    void RenderLayers();
    void ShutdownLayers();

public:
    template <typename LayerImpl, typename... Args>
        requires std::derived_from<LayerImpl, ILayer<ApplicationState>> && requires {
            { LayerImpl::GetLayerName() } -> std::convertible_to<std::string_view>;
        }
    LayerImpl& PushLayer(Args&&... args);

protected:
    WindowConfiguration m_window_configuration{};
    ApplicationState m_app_state{};

private:
    std::vector<typename ILayer<ApplicationState>::Ptr> m_layers{};
    std::unique_ptr<Graphite::Core::Renderer::IRenderer> m_renderer{nullptr};
};

#pragma region LifeCycle
template <typename ApplicationState>
GraphiteApplication<ApplicationState>::GraphiteApplication(
    WindowConfiguration window_configuration,
    ApplicationState initial_state)
    : m_window_configuration{std::move(window_configuration)}, m_app_state{std::move(initial_state)}
{
    LOG_SCOPE("");
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::Run()
{
    LOG_SCOPE("");
    Init();
    m_renderer->Render(this->shared_from_this());
    Shutdown();
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::Init()
{
    LOG_SCOPE("");
    m_renderer = Graphite::Core::Renderer::CreateRenderer();
    m_renderer->Init();
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::Render()
{
    LOG_SCOPE("");
    RenderLayers();
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::Shutdown()
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
LayerImpl& GraphiteApplication<ApplicationState>::PushLayer(Args&&... args)
{
    LOG_SCOPE("{}", LayerImpl::GetLayerName().data());
    auto layer = std::make_unique<LayerImpl>(std::forward<Args>(args)...);
    layer->OnPush(m_app_state);
    m_layers.push_back(std::move(layer));
    return static_cast<LayerImpl&>(*m_layers.back());
}

template <typename ApplicationState>
ApplicationState& GraphiteApplication<ApplicationState>::GetApplicationState()
{
    return m_app_state;
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::RenderLayers()
{
    LOG_SCOPE("");
    std::for_each(m_layers.begin(), m_layers.end(), [this](ILayer<ApplicationState>::Ptr& layer_ptr) {
        layer_ptr->OnRender(m_app_state);
    });
}

template <typename ApplicationState>
void GraphiteApplication<ApplicationState>::ShutdownLayers()
{
    LOG_SCOPE("");
    std::for_each(m_layers.begin(), m_layers.end(), [this](ILayer<ApplicationState>::Ptr& layer_ptr) {
        layer_ptr->OnShutdown(m_app_state);
    });
}
#pragma endregion Internals

} // namespace Graphite::Core::Application
