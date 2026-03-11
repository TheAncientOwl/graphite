/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file BasicTableApp.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Playground.
///

#include <algorithm>
#include <optional>
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
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

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
    }

private: // UI
    void CleanupBanned()
    {
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
    }

    void RenderPlayersSelect()
    {
        ImGui::BeginChild("LeftSidebar", ImVec2(80, 0), ImGuiChildFlags_Borders);
        for (std::size_t playerIndex = 0; playerIndex < m_state.players.size(); playerIndex++)
        {
            if (!IsRenderable(m_state.players[playerIndex]))
            {
                continue;
            }

            ImGui::PushID(playerIndex);
            if (ImGui::Selectable(m_state.players[playerIndex].name.c_str()) &&
                (!static_cast<bool>(m_state.selected_player) ||
                 (static_cast<bool>(m_state.selected_player) &&
                  *m_state.selected_player != playerIndex)))
            {
                SetEditPlayer(playerIndex);
            }
            ImGui::PopID();
        }
        ImGui::EndChild();
    }

    void RenderPlayersTable()
    {
        // 1. Setup the table (3 columns)
        ImGui::BeginChild("TableRegion");
        if (ImGui::BeginTable("PlayerTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
        {
            // 2. Setup Headers
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Health");
            ImGui::TableSetupColumn("Action [-]");
            ImGui::TableSetupColumn("Action [+]");
            ImGui::TableSetupColumn("Action [x]");
            ImGui::TableHeadersRow();

            // 3. Populate Rows
            std::unordered_set<std::size_t> banned_indices{};
            for (std::size_t playerIndex = 0; playerIndex < m_state.players.size(); playerIndex++)
            {
                auto& player = m_state.players[playerIndex];

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

                ImGui::PushID(playerIndex); // Important for button uniqueness!
                // Column 2: Action [-]
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{0.0f, 0.0f, 0.0f, 1.0f}); // text color

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.45f, 0.0f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.8f, 0.45f, 0.0f, 1.0f});
                if (ImGui::Button("Kick"))
                {
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
                }
                ImGui::PopStyleColor(2); // kick button

                // Column 3: Action [+]
                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.25f, 1.0f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.75f, 1.0f, 0.0f, 1.0f});
                if (ImGui::Button("Heal"))
                {
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
                }
                ImGui::PopStyleColor(2); // heal button

                // Column 4: Action [x]
                ImGui::TableSetColumnIndex(4);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.25f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1.0f, 0.75f, 0.0f, 1.0f});
                if (ImGui::Button("Ban"))
                {
                    LOG_INFO(
                        "Shallow Banned player {}::{}", playerIndex, m_state.players[playerIndex].name);
                    m_state.players[playerIndex].banned = true;
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
        }

        ImGui::SliderInt("Health", &m_state.edits.health, 0, 100);
        if (ImGui::IsItemDeactivatedAfterEdit())
        {
            LOG_INFO(
                "Saving player health -> prev{} -> now{}",
                m_state.players[idx].health,
                m_state.edits.health);
            m_state.players[idx].health = m_state.edits.health;
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

private:
    struct Player
    {
        std::string name{""};
        int health{0};
        bool banned{false};
    };

private: // Utils
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
        bool show_players{true};
        std::optional<std::size_t> selected_player{std::nullopt};
        std::vector<Player> players{
            {"Player1", 75},
            {"Player2", 45},
            {"Player3", 100},
            {"Player4", 75},
            {"Player5", 23},
            {"Player6", 95},
            {"Player7", 73},
            {"Player8", 47},
            {"Player9", 29},
        };
    };

private: // Fields
    State m_state{};
};

} // namespace Graphite::Demo
