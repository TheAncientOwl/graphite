/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file main.cpp
/// @author Alexandru Delegeanu
/// @version 0.4
/// @brief ImGui entry point.
///

#include <memory>
#include <stdlib.h>

#include "imgui/imgui.h"

#include "core/logger/Logger.hpp"
#include "project/PlayersApplication.hpp"

int main()
{
    LOG_SCOPE("");

    Graphite::Core::Application::WindowConfiguration window_configuration{};
    window_configuration.width = 500;
    window_configuration.height = 600;
    window_configuration.name = "Some Application";

    auto app = std::make_shared<Graphite::Project::PlayersApplication>(
        window_configuration, Graphite::Project::AppState{});
    app->Run();

    return EXIT_SUCCESS;
}
