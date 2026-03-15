/// --------------------------------------------------------------------------
///                     Copyright (c) by Graphite 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/graphite/blob/main/LICENSE
///
/// @file Calculator.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see Calculator.hpp.
///

#include "CalculatorApp.hpp"

#include <exception>

#include "Core/Logger/Logger.hpp"

namespace Graphite::Demo::Calculator {

CalculatorApp::CalculatorApp()
{
    LOG_SCOPE("");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    m_font = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto_Condensed-Medium.ttf", 28.0f);
}

void CalculatorApp::Render()
{
    LOG_SCOPE("");
    switch (m_app_state)
    {
    case EAppState::Calculator:
        LOG_TRACE("Rendering Calculator state");
        ImGui::PushFont(m_font);
        States::DrawCalculator(m_calculator_state);
        ImGui::PopFont();
        break;
    default:
        LOG_ERROR("Rendering not supported state");
        break;
    }
}

} // namespace Graphite::Demo::Calculator
