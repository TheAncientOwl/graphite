/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file DummyPlugin.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Dummy plugin.
///

#include "IPlugin.hpp"

#include "core/lib-utils/GraphiteExport.hpp"

#include "core/logger/Logger.hpp"

namespace Graphite::Core::Plugin {

class DummyPlugin : public IGraphitePlugin
{
public:
    DummyPlugin() { LOG_SCOPE(""); }
    void Execute() override { LOG_SCOPE(""); }
    std::string_view GetName() override { return "DummyPlugin"; }
    ~DummyPlugin() { LOG_SCOPE(""); }
};

} // namespace Graphite::Core::Plugin

extern "C" GRAPHITE_EXPORT Graphite::Core::Plugin::IGraphitePlugin* CreateGraphitePlugin()
{
    return new Graphite::Core::Plugin::DummyPlugin();
}
