/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file Playground.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Initial version of graphite app ~ ImGui demo.
///

#include "imgui/imgui.h"

#include "Core/Logger/Logger.hpp"
#include "Core/Renderer/Renderer.hpp"

namespace Graphite::Demo {

class Playground : public Graphite::Core::Renderer::IRenderable
{
public:
    void Render() override
    {
        ImGui::Begin("Main Editor");

        // Left Side: Sidebar
        ImGui::BeginChild("Sidebar", ImVec2(150, 0), ImGuiChildFlags_Borders);
        for (int i = 0; i < 20; i++)
        {
            ImGui::PushID(i);
            ImGui::Selectable("Object");
            ImGui::PopID();
        }
        ImGui::EndChild();

        ImGui::SameLine();

        // Right Side: Main Content
        ImGui::BeginChild("Content Area", ImVec2(-1, -1), ImGuiChildFlags_Borders);
        ImGui::Text("Details go here...");
        ImGui::EndChild();

        ImGui::End();

        //
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Position it at the very bottom
        ImVec2 work_pos = viewport->WorkPos;
        ImVec2 work_size = viewport->WorkSize;
        float bar_height = 30.0f;

        ImGui::SetNextWindowPos(ImVec2(work_pos.x, work_pos.y + work_size.y - bar_height));
        ImGui::SetNextWindowSize(ImVec2(work_size.x, bar_height));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
                                 ImGuiWindowFlags_NoSavedSettings;

        if (ImGui::Begin("##StatusBar", nullptr, flags))
        {
            ImGui::Text("Application Running | FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();
        }
    }

private:
    struct State
    {
    };
    State m_state{};
};

} // namespace Graphite::Demo
