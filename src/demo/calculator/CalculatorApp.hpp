/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Calculator.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Demo calculator app.
///

#include "Core/Application/Renderer/Renderer.hpp"
#include "demo/calculator/states/Calculator.hpp"

#include "imgui/imgui.h"

namespace Graphite::Demo::Calculator {

class CalculatorApp : public Core::Renderer::IRenderable
{
public:
    enum class EAppState : std::uint8_t
    {
        Calculator = 0
    };

public:
    CalculatorApp();
    void Render() override;

private:
    EAppState m_app_state{EAppState::Calculator};
    States::CalculatorState m_calculator_state{};
    ImFont* m_font{nullptr};
};

} // namespace Graphite::Demo::Calculator
