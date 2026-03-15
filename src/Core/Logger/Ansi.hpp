/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Ansi.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief ANSI helpers for escape sequences.
///

#pragma once

#include <ostream>
#include <string>
#include <string_view>

namespace Graphite::Core::Logger::Ansi {

void writeWithoutAnsi(std::ostream& os, std::string_view s) noexcept;

} // namespace Graphite::Core::Logger::Ansi
