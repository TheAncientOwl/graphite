/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file IPlugin.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Test plugin interface.
///

#include <memory>
#include <string_view>

#if defined(_WIN32)
#define GRAPHITE_EXPORT __declspec(dllexport)
#else
#define GRAPHITE_EXPORT __attribute__((visibility("default")))
#endif

namespace Graphite::Core::Plugin {

class IGraphitePlugin
{
public:
    virtual void Execute() = 0;
    virtual std::string_view GetName() = 0;
    virtual ~IGraphitePlugin() {}
};

}; // namespace Graphite::Core::Plugin

extern "C" GRAPHITE_EXPORT Graphite::Core::Plugin::IGraphitePlugin* CreateGraphitePlugin();
typedef Graphite::Core::Plugin::IGraphitePlugin* (*CreateGraphitePluginFactory)();
