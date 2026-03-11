/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file main.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief ImGui entry point.
///

#include <stdlib.h>

#include "imgui/imgui.h"

#include "core/logger/Logger.hpp"
#include "core/renderer/Renderer.hpp"

#include "demo/BasicTableApp.hpp"

int main()
{
    LOG_SCOPE("");

    auto app = std::make_shared<Graphite::Demo::BasicTableApp>();

    auto renderer = Graphite::Core::Renderer::CreateRenderer();
    renderer->Init();
    renderer->Render(app);
    renderer->Cleanup();

    return EXIT_SUCCESS;
}
