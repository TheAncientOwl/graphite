/// --------------------------------------------------------------------------
///                     Copyright (c) by Graphite 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/graphite/blob/main/LICENSE
///
/// @file DemoAppV1.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Initial version of graphite app ~ ImGui demo.
///

#include "imgui/imgui.h"

#include "Core/Logger/Logger.hpp"
#include "Core/Renderer/Renderer.hpp"

namespace Graphite::Demo {

class DemoAppV1 : public Graphite::Core::Renderer::IRenderable
{
public:
    DemoAppV1()
    {
        LOG_TRACE(">> trace: {}", std::rand());
        LOG_INFO(">> info: {}", std::rand());
        LOG_WARN(">> warn: {}", std::rand());
        LOG_ERROR(">> error: {}", std::rand());
        LOG_CRITICAL(">> critical: {}", std::rand());
        LOG_DEBUG(">> debug: {}", std::rand());
    }

    void Render() override
    {
        LOG_SCOPE("");

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!
        // You can browse its code to learn more about Dear ImGui!).
        if (m_state.show_demo_window)
        {
            ImGui::ShowDemoWindow(&m_state.show_demo_window);
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text(
                "This is some useful text."); // Display some text (you can use a format strings too)
            ImGui::Checkbox(
                "Demo Window",
                &m_state.show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &m_state.show_another_window);

            ImGui::SliderFloat(
                "float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3(
                "clear color", (float*)&m_state.clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGuiIO& io = ImGui::GetIO();
            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (m_state.show_another_window)
        {
            ImGui::Begin(
                "Another Window",
                &m_state.show_another_window); // Pass a pointer to our bool variable (the window will
                                               // have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                m_state.show_another_window = false;
            ImGui::End();
        }
    }

private:
    struct State
    {
        bool show_demo_window{true};
        bool show_another_window{false};
        float clear_color[4]{0.45f, 0.55f, 0.60f, 1.00f};
    };
    State m_state{};
};

} // namespace Graphite::Demo
