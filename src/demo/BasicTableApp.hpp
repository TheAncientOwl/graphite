/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file BasicTableApp.hpp
/// @author Alexandru Delegeanu
/// @version 0.7
/// @brief Playground.
///

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <optional>
#include <random>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

#include "imgui/imgui.h"

#include "core/logger/Logger.hpp"
#include "core/renderer/Renderer.hpp"

namespace Graphite::Demo {

class BasicTableApp : public Graphite::Core::Renderer::IRenderable
{
public:
    BasicTableApp()
    {
        LoadPlayers();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ApplyTheme();
    }

    ~BasicTableApp() { SavePlayers(); }

public:
    void Render() override
    {
        RenderMenuBar();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

        if (!m_state.show_players)
        {
            return;
        }

        ImGui::Begin("Players", &m_state.show_players);

        ImGui::Text("Server Status: Online");
        ImGui::Separator();

        RenderSearchFilter();
        ImGui::Separator();

        RenderPlayersSelect();

        ImGui::SameLine();

        RenderPlayersTable();

        RenderEditSelectedPlayer();

        ImGui::End();

        CleanupBanned();
        SavePlayers();
    }

private: // UI
    void RenderPlayersSelect()
    {
        ImGui::BeginChild("LeftSidebar", ImVec2(80, 0), ImGuiChildFlags_Borders);
        for (std::size_t player_index = 0; player_index < m_state.players.size(); player_index++)
        {
            if (!IsRenderable(m_state.players[player_index]))
            {
                continue;
            }

            ImGui::PushID(player_index);
            if (ImGui::Selectable(m_state.players[player_index].name.c_str()) &&
                (!static_cast<bool>(m_state.selected_player) ||
                 (static_cast<bool>(m_state.selected_player) &&
                  *m_state.selected_player != player_index)))
            {
                SetEditPlayer(player_index);
            }
            ImGui::PopID();

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                // 1. Attach the payload identifier ("DND_PLAYER") and data
                ImGui::SetDragDropPayload("DND_PLAYER", &player_index, sizeof(std::size_t));

                // 2. Display a helpful tooltip while dragging
                ImGui::Text("%s", m_state.players[player_index].name.c_str());

                ImGui::EndDragDropSource();
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PLAYER"))
                {
                    LOG_SCOPE("Move player with D&D");
                    auto const source_display_idx = *(const std::size_t*)payload->Data;
                    auto const target_display_idx = player_index;
                    LOG_INFO(
                        "Move player: {} before player {}",
                        m_state.players[source_display_idx].name,
                        m_state.players[target_display_idx].name);

                    if (source_display_idx != target_display_idx)
                    {
                        // 1. Move the Player object in the raw vector
                        auto moved_player = std::move(m_state.players[source_display_idx]);
                        m_state.players.erase(m_state.players.begin() + source_display_idx);
                        m_state.players.insert(
                            m_state.players.begin() + target_display_idx, std::move(moved_player));

                        // 2. Reset indices to match the new physical order (0, 1, 2...)
                        std::iota(
                            m_state.sorted_players_indices.begin(),
                            m_state.sorted_players_indices.end(),
                            0);

                        m_state.save_players_data = true;
                        m_state.reorder_players_data = true;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }

        ImGui::EndChild();
    }

    void RenderPlayersTable()
    {
        // 1. Setup the table (3 columns)
        ImGui::BeginChild("TableRegion");
        if (ImGui::BeginTable(
                "PlayerTable",
                5,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable))
        {
            // 2. Setup Headers
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn("Action [-]");
            ImGui::TableSetupColumn("Action [+]");
            ImGui::TableSetupColumn("Action [x]");
            ImGui::TableHeadersRow();

            if (auto specs = ImGui::TableGetSortSpecs();
                m_state.reorder_players_data || specs->SpecsDirty)
            {
                LOG_SCOPE("Sorting data");
                const ImGuiTableColumnSortSpecs* sort_spec = &specs->Specs[0];

                std::sort(
                    m_state.sorted_players_indices.begin(),
                    m_state.sorted_players_indices.end(),
                    [&](std::size_t const a, std::size_t const b) {
                        const auto& player1 = m_state.players[a];
                        const auto& player2 = m_state.players[b];
                        bool res = false;

                        switch (sort_spec->ColumnIndex)
                        {
                        case 0: {
                            auto const cmp = player1.name < player2.name;
                            res = (player1.name != player2.name) ? cmp : a < b;
                            break;
                        }
                        case 1: {
                            res = (player1.health != player2.health) ? player1.health < player2.health
                                                                     : a < b;
                            break;
                        }
                        }

                        return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? res : !res;
                    });

                specs->SpecsDirty = false;
                m_state.reorder_players_data = false;
            }

            // 3. Populate Rows
            std::unordered_set<std::size_t> banned_indices{};
            for (auto player_index : m_state.sorted_players_indices)
            {
                auto& player = m_state.players[player_index];

                if (!IsRenderable(player))
                {
                    continue;
                }

                ImGui::TableNextRow();

                // Column 0: Name
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", player.name.c_str());

                // Column 1: Health
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d HP", player.health);

                ImGui::PushID(player_index); // Important for button uniqueness!
                // Column 2: Action [-]
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.0f, 0.0f, 0.0f, 1.0f}); // text color

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.45f, 0.0f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.45f, 0.0f, 1.0f});
                if (ImGui::Button("Kick"))
                {
                    m_state.save_players_data = true;
                    player.health -= 5;
                    if (player.health < 0)
                    {
                        player.health = 0;
                        LOG_WARN("Can't take kicks OMG HE'S 0 HP!!!!");
                    }
                    else
                    {
                        LOG_INFO("Kick {}! -5hp", player.name);
                    }
                    m_state.reorder_players_data = true;
                }
                ImGui::PopStyleColor(2); // kick button

                // Column 3: Action [+]
                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.25f, 1.0f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.75f, 1.0f, 0.0f, 1.0f});
                if (ImGui::Button("Heal"))
                {
                    m_state.save_players_data = true;
                    player.health += 5;
                    if (player.health > 100)
                    {
                        player.health = 100;
                        LOG_WARN("Can't be healed no more OMG HE'S 100 HP!!!!");
                    }
                    else
                    {
                        LOG_INFO("Heal {}! +5hp", player.name);
                    }
                    m_state.reorder_players_data = true;
                }
                ImGui::PopStyleColor(2); // heal button

                // Column 4: Action [x]
                ImGui::TableSetColumnIndex(4);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.25f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1.0f, 0.75f, 0.0f, 1.0f});
                if (ImGui::Button("Ban"))
                {
                    m_state.save_players_data = true;
                    LOG_INFO(
                        "Shallow Banned player {}::{}", player_index, m_state.players[player_index].name);
                    m_state.players[player_index].banned = true;
                }
                ImGui::PopStyleColor(2); // ban button

                ImGui::PopStyleColor(1); // text color

                ImGui::PopID();
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }

    void RenderEditSelectedPlayer()
    {
        if (!m_state.selected_player)
            return;

        auto const idx{*m_state.selected_player};

        ImGui::Begin("Edit Player");

        if (ImGui::InputText("Edit Name", m_state.edits.buffer, sizeof(m_state.edits.buffer)))
        {
            LOG_INFO(
                "Saving player name -> prev{} -> now{}", m_state.players[idx].name, m_state.edits.buffer);
            m_state.players[idx].name = m_state.edits.buffer;
            m_state.save_players_data = true;
            m_state.reorder_players_data = true;
        }

        ImGui::SliderInt("Health", &m_state.edits.health, 0, 100);
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            LOG_INFO(
                "Saving player health -> prev{} -> now{}",
                m_state.players[idx].health,
                m_state.edits.health);
            m_state.players[idx].health = m_state.edits.health;
            m_state.save_players_data = true;
            m_state.reorder_players_data = true;
        }

        if (ImGui::Button("Done"))
        {
            LOG_INFO("Done editing");
            m_state.selected_player = std::nullopt;
        }

        ImGui::End();
    }

    void RenderMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Player", "Ctrl+N"))
                {
                    m_state.players.emplace_back("Dummy", 0);
                    m_state.sorted_players_indices.push_back(m_state.players.size() - 1);
                    m_state.save_players_data = true;
                    m_state.reorder_players_data = true;
                    SetEditPlayer(m_state.players.size() - 1);
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Quit", "Alt+F4"))
                { /* Logic */
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {
                ImGui::MenuItem("Show Player List", nullptr, &m_state.show_players);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void RenderSearchFilter()
    {
        if (ImGui::InputText("Search Player", m_state.filter.buffer, sizeof(m_state.filter.buffer)))
        {
            m_state.filter.length = strlen(m_state.filter.buffer);

            if (static_cast<bool>(m_state.selected_player))
            {
                if (!NameMatchesSearch(m_state.players[*m_state.selected_player].name))
                {
                    m_state.selected_player = std::nullopt;
                }
            }
        }
    }

private: // Player data structure
    struct Player
    {
        std::string name{""};
        int health{0};
        bool banned{false};

        Player() = default;
        Player(std::string name, int const health)
            : name{std::move(name)}, health{health}, banned{false}
        {
        }
        Player(std::istream& is) { is >> *this; }

        friend std::ostream& operator<<(std::ostream& os, Player const& player)
        {
            return os << player.name << '\n' << player.health << '\n';
        }

        friend std::istream& operator>>(std::istream& is, Player& player)
        {
            player.banned = false;

            std::getline(is, player.name);
            is >> player.health;
            is.get(); // read newline char
            return is;
        }
    };

private: // Utils
    void CleanupBanned()
    {
        auto const initial_size{m_state.players.size()};
        // 1. If we have a selection, calculate the new index
        if (m_state.selected_player.has_value())
        {
            std::size_t old_idx = *m_state.selected_player;
            std::size_t offset = 0;

            for (std::size_t i = 0; i < old_idx; ++i)
            {
                if (m_state.players[i].banned)
                    offset++;
            }

            // 2. If the selected player themselves is banned, clear the selection
            if (m_state.players[old_idx].banned)
            {
                m_state.selected_player = std::nullopt;
            }
            else
            {
                *m_state.selected_player -= offset;
            }
        }

        // 3. Actually remove the players from the vector
        m_state.players.erase(
            std::remove_if(
                m_state.players.begin(),
                m_state.players.end(),
                [](Player const& p) { return p.banned; }),
            m_state.players.end());

        bool const data_changed{m_state.players.size() != initial_size};

        m_state.save_players_data |= data_changed;

        if (data_changed)
        {
            m_state.sorted_players_indices.resize(m_state.players.size());
            std::iota(m_state.sorted_players_indices.begin(), m_state.sorted_players_indices.end(), 0);
            m_state.reorder_players_data = true;
        }
    }

    void SetEditPlayer(std::size_t const index)
    {
        m_state.selected_player = index;

        strncpy(
            m_state.edits.buffer, m_state.players[index].name.c_str(), sizeof(m_state.edits.buffer));

        m_state.edits.health = m_state.players[index].health;
    }

    bool NameMatchesSearch(std::string_view const name) const
    {
        return m_state.filter.length == 0 || name.contains(m_state.filter.buffer);
    }

    bool IsRenderable(Player const& player) const
    {
        return !player.banned && NameMatchesSearch(player.name);
    }

    static inline constexpr std::filesystem::path getPlayersDataPath()
    {
        return std::filesystem::current_path() / "players.txt";
    }

    void SavePlayers()
    {
        if (!m_state.save_players_data)
        {
            return;
        }
        LOG_SCOPE("");

        std::ofstream file{getPlayersDataPath(), std::ios::trunc};
        LOG_INFO("Saving {} players", m_state.players.size());
        file << m_state.players.size() << '\n';
        std::for_each(m_state.players.begin(), m_state.players.end(), [&](Player const& player) {
            file << player;
        });

        m_state.save_players_data = false;
    }

    void LoadPlayers()
    {
        LOG_SCOPE("");

        auto const path{getPlayersDataPath()};

        if (!std::filesystem::exists(path))
        {
            LOG_INFO("Initial app run, setting dummy data");
            m_state.save_players_data = true;
            static constexpr std::size_t default_players = 10000;

            std::mt19937 rng{std::random_device{}()};
            std::uniform_int_distribution<int> health_dist(0, 100);

            m_state.players.reserve(default_players);
            m_state.sorted_players_indices.reserve(default_players);

            for (std::size_t i = 0; i < default_players; ++i)
            {
                std::string name = "Player" + std::to_string(i + 1);
                int health = health_dist(rng);

                m_state.players.emplace_back(name, health);
                m_state.sorted_players_indices.push_back(i);
            }
            return;
        }

        LOG_INFO("Loading players...");

        std::ifstream file{path};

        std::size_t players_count{0};
        file >> players_count;
        file.get(); // read newline char
        LOG_INFO("Players count to load: {}", players_count);
        m_state.players.reserve(players_count);

        for (auto const player_index : std::ranges::iota_view(std::size_t{0}, players_count))
        {
            m_state.players.emplace_back(file);
            m_state.sorted_players_indices.push_back(player_index);
        }
    }

    void ApplyTheme()
    {
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

private: // Data Structures
    struct State
    {
        struct
        {
            char buffer[64];
            int health{0};
        } edits{};
        struct
        {
            char buffer[64];
            std::size_t length{0};
        } filter{};
        bool save_players_data{false};
        bool reorder_players_data{false};
        bool show_players{true};
        std::optional<std::size_t> selected_player{std::nullopt};
        std::vector<Player> players{};
        std::vector<std::size_t> sorted_players_indices{};
    };

private: // Fields
    State m_state{};
};

} // namespace Graphite::Demo
