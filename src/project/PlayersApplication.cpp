/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file PlayersApplication.cpp
/// @author Alexandru Delegeanu
/// @version 0.2
/// @brief Implementation of @see PlayersApplication.hpp.
///

#include <numeric>
#include <random>
#include <ranges>

#include "imgui/imgui.h"

#include "PlayersApplication.hpp"
#include "layers/MainLayer.hpp"

namespace Graphite::Project {

PlayersApplication::PlayersApplication(
    Graphite::Core::Application::WindowConfiguration window_configuration,
    AppState initial_state)
    : TGraphiteApplication{std::move(window_configuration), std::move(initial_state)}
{
    LOG_SCOPE("");
}

PlayersApplication::~PlayersApplication()
{
    LOG_SCOPE("");
}

void PlayersApplication::AppInit()
{
    LoadPlayers();
    if (ImGui::GetCurrentContext() == nullptr)
    {
        ImGui::CreateContext();
    }
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ApplyTheme();

    PushLayer<Layers::MainLayer>(shared_from_this());
}

void PlayersApplication::LoadPlayers()
{
    LOG_SCOPE("");

    auto const path{GetPlayersDataPath()};

    if (!std::filesystem::exists(path))
    {
        LOG_INFO("Initial app run, setting dummy data");
        m_app_state.save_players_data = true;
        static constexpr std::size_t default_players = 10000;

        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> health_dist(0, 100);

        m_app_state.players.reserve(default_players);
        m_app_state.sorted_players_indices.reserve(default_players);

        for (std::size_t i = 0; i < default_players; ++i)
        {
            std::string name = "Player" + std::to_string(i + 1);
            int health = health_dist(rng);

            m_app_state.players.emplace_back(name, health);
            m_app_state.sorted_players_indices.push_back(i);
        }
        return;
    }

    LOG_INFO("Loading players...");

    std::ifstream file{path};

    std::size_t players_count{0};
    file >> players_count;
    file.get(); // read newline char
    LOG_INFO("Players count to load: {}", players_count);
    m_app_state.players.reserve(players_count);

    for (auto const player_index : std::ranges::iota_view(std::size_t{0}, players_count))
    {
        m_app_state.players.emplace_back(file);
        m_app_state.sorted_players_indices.push_back(player_index);
    }
}

void PlayersApplication::ApplyTheme()
{
    LOG_SCOPE("");

    // >> Spacings
    ImGuiStyle& style = ImGui::GetStyle();

    // Soften the corners (Modern look)
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 4.0f;

    // Add some breathing room
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 4.0f);
    style.ItemSpacing = ImVec2(8.0f, 8.0f);

    // Make borders subtle
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;

    // >> Colors
    ImGui::StyleColorsDark(); // Start with default dark theme
    ImVec4* colors = ImGui::GetStyle().Colors;

    // Darken the background
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);

    // Change the primary accent color (e.g., a nice graphite/blue)
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.30f, 0.35f, 1.0f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.35f, 0.40f, 1.0f);

    // Base button colors
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.25f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.28f, 0.32f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.35f, 0.40f, 1.0f);

    // Frames(InputText, etc)
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.0f);

    // >> Fonts
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto_Condensed-Medium.ttf", 15.0f);

    // >> Colors theme
    ImGui::StyleColorsDark(); // Start with default dark theme
    // ImVec4* colors = ImGui::GetStyle().Colors;

    // --- The "Primary" Green Accents ---
    const ImVec4 leafGreen = ImVec4(0.15f, 0.55f, 0.38f, 1.00f);
    const ImVec4 leafGreenHovered = ImVec4(0.18f, 0.65f, 0.45f, 1.00f);
    const ImVec4 leafGreenActive = ImVec4(0.20f, 0.75f, 0.50f, 1.00f);

    // 1. Headers (Collapsing headers, Table headers)
    colors[ImGuiCol_Header] = leafGreen;
    colors[ImGuiCol_HeaderHovered] = leafGreenHovered;
    colors[ImGuiCol_HeaderActive] = leafGreenActive;

    // 2. Buttons (Default state)
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.22f, 1.00f); // Dark mossy base
    colors[ImGuiCol_ButtonHovered] = leafGreenHovered;
    colors[ImGuiCol_ButtonActive] = leafGreenActive;

    // 3. Selection & Checkmarks
    colors[ImGuiCol_FrameBgActive] = leafGreen; // Input fields when clicked
    colors[ImGuiCol_CheckMark] = leafGreenActive;
    colors[ImGuiCol_SliderGrab] = leafGreen;
    colors[ImGuiCol_SliderGrabActive] = leafGreenActive;

    // 4. Tabs (For Docking)
    colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.18f, 0.14f, 1.00f);
    colors[ImGuiCol_TabHovered] = leafGreenHovered;
    colors[ImGuiCol_TabActive] = leafGreen;
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.12f, 0.25f, 0.20f, 1.00f);

    // 5. Window Titles (The "Blue" bar at the top of windows)
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.15f, 0.13f, 1.00f); // Subtle dark green-grey
    colors[ImGuiCol_SeparatorHovered] = leafGreenHovered;
    colors[ImGuiCol_SeparatorActive] = leafGreenActive;

    // --- Input Widget Backgrounds ---
    // The default state of the input box
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.15f, 0.13f, 1.00f);
    // When you hover over the input
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.20f, 0.17f, 1.00f);
    // When you click/type in the input
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.25f, 0.21f, 1.00f);

    // --- Slider & Checkbox Specifics ---
    // The "knob" on the slider
    colors[ImGuiCol_SliderGrab] = ImVec4(0.15f, 0.55f, 0.38f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.20f, 0.75f, 0.50f, 1.00f);
    // The checkmark in a checkbox
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.75f, 0.50f, 1.00f);
}

} // namespace Graphite::Project
