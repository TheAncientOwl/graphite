/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file IPlugin.hpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Test plugin interface.
///

#include <memory>
#include <string_view>

#include "Core/plugin/GraphiteExport.hpp"

namespace Graphite::Core::Plugin {

class IGraphitePlugin
{
public:
    virtual void Execute() = 0;
    virtual std::string_view GetName() = 0;
    virtual ~IGraphitePlugin() = default;
};

}; // namespace Graphite::Core::Plugin

extern "C" GRAPHITE_EXPORT Graphite::Core::Plugin::IGraphitePlugin* CreateGraphitePlugin();
typedef Graphite::Core::Plugin::IGraphitePlugin* (*CreateGraphitePluginFactory)();
