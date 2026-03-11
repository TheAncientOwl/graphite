/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file WindowConfiguration.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Configuration of graphite app window.
///

#include <cstdint>
#include <string>

namespace Graphite::Core::Application {

struct WindowConfiguration
{
    std::uint32_t width{0};
    std::uint32_t height{0};
    std::string name{};
};

} // namespace Graphite::Core::Application
