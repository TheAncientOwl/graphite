/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file main_plugins.cpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Plugins test entry point.
///

#include <memory>
#include <stdlib.h>

#include "imgui/imgui.h"

#include "Core/Application/Renderer/Renderer.hpp"
#include "Core/Logger/Logger.hpp"

#include "Core/plugin/DynamicLibrary.hpp"
#include "Core/plugin/IPlugin.hpp"

int main()
{
    LOG_SCOPE("");

#ifdef _WIN32
    auto const lib_path{std::filesystem::current_path() / "plugins" / "libdummy_plugin.dll"};
#elif defined(__APPLE__)
    auto const lib_path{std::filesystem::current_path() / "plugins" / "libdummy_plugin.dylib"};
#else
    auto const lib_path{std::filesystem::current_path() / "plugins" / "libdummy_plugin.so"};
#endif

    Graphite::Core::Plugin::DynamicLibrary dummyLib(lib_path);
    if (dummyLib.isLoaded())
    {
        auto factory = (CreateGraphitePluginFactory)dummyLib.getSymbol("CreateGraphitePlugin");
        if (factory)
        {
            auto plugin = std::unique_ptr<Graphite::Core::Plugin::IGraphitePlugin>(factory());
            LOG_INFO("Successfully loaded {}!", plugin->GetName());
            plugin->Execute();
        }
        else
        {
            LOG_CRITICAL("Failed to load symbol CreateGraphitePlugin at {}", lib_path.c_str());
            return EXIT_FAILURE;
        }
    }
    else
    {
        LOG_CRITICAL("Could not load dummy lib at {}", lib_path.c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
