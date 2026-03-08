/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Calculator.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Main calculator view.
///

#include <string>

namespace Graphite::Demo::Calculator::States {

struct CalculatorState
{
    using EquationType = std::string;
    EquationType equation{};
};

void DrawCalculator(CalculatorState& state);

} // namespace Graphite::Demo::Calculator::States
