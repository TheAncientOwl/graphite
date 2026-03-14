/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file main.cpp
/// @author Alexandru Delegeanu
/// @version 0.3
/// @brief ImGui entry point.
///

#include <memory>
#include <stdlib.h>

#include "imgui/imgui.h"

#include "core/logger/Logger.hpp"
#include "core/renderer/Renderer.hpp"

#include "demo/BasicTableApp.hpp"

#include "core/application/GraphiteApplication.hpp"

struct DummyState
{
    char buffer[64];
};

struct MainState
{
    char buffer[64];
    int some_number{101};
};

struct AppState
{
    MainState main{};
    DummyState dummy1{};
    DummyState dummy2{};
};

class MainLayer : public Graphite::Core::Application::ILayer<AppState>
{
public:
    static std::string_view GetLayerName() { return "MainLayer"; }

    MainLayer(MainState& state) : m_state{state} { LOG_SCOPE(""); }

    std::string_view GetName() override { return GetLayerName(); }
    void OnPush(AppState& app_state) override { LOG_SCOPE(""); };
    void OnPop(AppState& app_state) override { LOG_SCOPE(""); };
    void OnRender(AppState& app_state) override { LOG_SCOPE(""); };
    void OnShutdown(AppState& app_state) override { LOG_SCOPE(""); };

private:
    MainState& m_state;
};

class DummyLayer : public Graphite::Core::Application::ILayer<AppState>
{
public:
    static std::string_view GetLayerName() { return "DummyLayer"; }

    DummyLayer(DummyState state) : m_state{state} { LOG_SCOPE(""); }
    std::string_view GetName() override { return GetLayerName(); }
    void OnPush(AppState& app_state) override { LOG_SCOPE(""); };
    void OnPop(AppState& app_state) override { LOG_SCOPE(""); };
    void OnRender(AppState& app_state) override { LOG_SCOPE(""); };
    void OnShutdown(AppState& app_state) override { LOG_SCOPE(""); };

private:
    DummyState m_state;
};

int main()
{
    LOG_SCOPE("");

    Graphite::Core::Application::WindowConfiguration window_configuration{};
    window_configuration.width = 500;
    window_configuration.height = 600;
    window_configuration.name = "Some Application";

    auto app = std::make_shared<Graphite::Core::Application::GraphiteApplication<AppState>>(
        window_configuration, AppState{});
    app->PushLayer<MainLayer>(app->GetApplicationState().main);
    app->PushLayer<DummyLayer>(app->GetApplicationState().dummy1);
    app->PushLayer<DummyLayer>(app->GetApplicationState().dummy2);
    app->Run();

    return EXIT_SUCCESS;
}
