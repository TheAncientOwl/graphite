/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file MainLayer.cpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief Implementation of @see MainLayer.hpp.
///

#include <numeric>
#include <unordered_set>

#include "imgui/imgui.h"

#include "core/logger/Logger.hpp"
#include "project/PlayersApplication.hpp"

#include "MainLayer.hpp"

namespace Graphite::Project::Layers {

std::string_view MainLayer::GetLayerName() noexcept
{
    return "MainLayer";
}

MainLayer::MainLayer(Graphite::Project::PlayersApplication::Ptr application)
    : ILayer{std::move(application)}
{
    LOG_SCOPE("");
};

std::string_view MainLayer::GetName() const noexcept
{
    return GetLayerName();
}

void MainLayer::OnPush()
{
    LOG_SCOPE("");
}

void MainLayer::OnPop()
{
    LOG_SCOPE("");
    SavePlayers();
}

void MainLayer::OnRender()
{
    LOG_SCOPE("");

    RenderMenuBar();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

    auto& app_state{m_application->GetApplicationState()};

    if (!app_state.show_players)
    {
        return;
    }

    ImGui::Begin("Players", &app_state.show_players);

    ImGui::Text("Server Status: %zu Online", app_state.players.size());
    ImGui::Separator();

    RenderSearchFilter();
    ImGui::Separator();

    RenderPlayersSelect();

    ImGui::SameLine();

    RenderPlayersTable();

    RenderEditSelectedPlayer();

    ImGui::End();
}

void MainLayer::RenderPlayersSelect()
{
    LOG_SCOPE("");
    auto& app_state{m_application->GetApplicationState()};

    ImGui::BeginChild("LeftSidebar", ImVec2(80, 0), ImGuiChildFlags_Borders);

    ImGuiListClipper clipper{};
    clipper.Begin(static_cast<int>(app_state.sorted_players_indices.size()));

    while (clipper.Step())
    {
        // for (std::size_t player_index = 0; player_index < app_state.players.size(); player_index++)
        LOG_INFO("Rendering from {} to {}", clipper.DisplayStart, clipper.DisplayEnd);
        for (auto player_index = clipper.DisplayStart; player_index < clipper.DisplayEnd; ++player_index)
        {
            if (!IsRenderable(app_state.players[player_index]))
            {
                continue;
            }

            ImGui::PushID(player_index);
            if (ImGui::Selectable(app_state.players[player_index].name.c_str()) &&
                (!static_cast<bool>(app_state.selected_player) ||
                 (static_cast<bool>(app_state.selected_player) &&
                  *app_state.selected_player != player_index)))
            {
                SetEditPlayer(player_index);
            }
            ImGui::PopID();

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
            {
                // 1. Attach the payload identifier ("DND_PLAYER") and data
                ImGui::SetDragDropPayload("DND_PLAYER", &player_index, sizeof(std::size_t));

                // 2. Display a helpful tooltip while dragging
                ImGui::Text("%s", app_state.players[player_index].name.c_str());

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
                        app_state.players[source_display_idx].name,
                        app_state.players[target_display_idx].name);

                    if (source_display_idx != target_display_idx)
                    {
                        // 1. Move the Player object in the raw vector
                        auto moved_player = std::move(app_state.players[source_display_idx]);
                        app_state.players.erase(app_state.players.begin() + source_display_idx);
                        app_state.players.insert(
                            app_state.players.begin() + target_display_idx, std::move(moved_player));

                        // 2. Reset indices to match the new physical order (0, 1, 2...)
                        std::iota(
                            app_state.sorted_players_indices.begin(),
                            app_state.sorted_players_indices.end(),
                            0);

                        app_state.save_players_data = true;
                        app_state.reorder_players_data = true;
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
    }

    ImGui::EndChild();
}

void MainLayer::RenderPlayersTable()
{
    LOG_SCOPE("");
    auto& app_state{m_application->GetApplicationState()};

    // 1. Setup the table (3 columns)
    ImGui::BeginChild("TableRegion");
    if (ImGui::BeginTable(
            "PlayerTable",
            5,
            ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable |
                ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable |
                ImGuiTableFlags_Reorderable,
            ImVec2(0.0f, 0.0f)))
    {
        ImGui::TableSetupScrollFreeze(1, 1);

        // 2. Setup Headers
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Health", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Action [-]");
        ImGui::TableSetupColumn("Action [+]");
        ImGui::TableSetupColumn("Action [x]");
        ImGui::TableHeadersRow();

        if (auto specs = ImGui::TableGetSortSpecs(); app_state.reorder_players_data || specs->SpecsDirty)
        {
            LOG_SCOPE("Sorting data");
            const ImGuiTableColumnSortSpecs* sort_spec = &specs->Specs[0];

            std::sort(
                app_state.sorted_players_indices.begin(),
                app_state.sorted_players_indices.end(),
                [&](std::size_t const a, std::size_t const b) {
                    const auto& player1 = app_state.players[a];
                    const auto& player2 = app_state.players[b];
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
            app_state.reorder_players_data = false;
        }

        // 3. Populate Rows
        std::unordered_set<std::size_t> banned_indices{};

        ImGuiListClipper clipper{};
        clipper.Begin(static_cast<int>(app_state.sorted_players_indices.size()));

        while (clipper.Step())
        {
            LOG_INFO("Rendering from {} to {}", clipper.DisplayStart, clipper.DisplayEnd);

            // for (auto player_index : app_state.sorted_players_indices)
            for (auto player_index = clipper.DisplayStart; player_index < clipper.DisplayEnd;
                 ++player_index)
            {
                auto& player = app_state.players[player_index];

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
                    app_state.save_players_data = true;
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
                    app_state.reorder_players_data = true;
                }
                ImGui::PopStyleColor(2); // kick button

                // Column 3: Action [+]
                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.25f, 1.0f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.75f, 1.0f, 0.0f, 1.0f});
                if (ImGui::Button("Heal"))
                {
                    app_state.save_players_data = true;
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
                    app_state.reorder_players_data = true;
                }
                ImGui::PopStyleColor(2); // heal button

                // Column 4: Action [x]
                ImGui::TableSetColumnIndex(4);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.25f, 0.45f, 1.0f});
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1.0f, 0.75f, 0.0f, 1.0f});
                if (ImGui::Button("Ban"))
                {
                    app_state.open_ban_modal = true;
                    app_state.target_ban_player = player_index;
                }
                ImGui::PopStyleColor(2); // ban button

                ImGui::PopStyleColor(1);

                ImGui::PopID();
            }
        }

        ImGui::EndTable();
    }

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    if (app_state.open_ban_modal)
    {
        app_state.open_ban_modal = false;
        ImGui::OpenPopup("Confirm Ban?");
    }
    // This must use the EXACT same string ID as OpenPopup
    if (ImGui::BeginPopupModal("Confirm Ban?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (static_cast<bool>(app_state.target_ban_player))
        {
            ImGui::Text(
                "Are you sure you want to ban %s?\nThis action cannot be "
                "undone.",
                app_state.players[*app_state.target_ban_player].name.c_str());
            ImGui::Separator();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1.0f, 0.0f, 0.0f, 0.5f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1.0f, 0.0f, 0.0f, 0.75f});
            if (ImGui::Button("Yes, Ban Them", ImVec2(120, 0)))
            {
                app_state.save_players_data = true;
                LOG_INFO(
                    "Shallow Banned player {}::{}",
                    *app_state.target_ban_player,
                    app_state.players[*app_state.target_ban_player].name);
                app_state.players[*app_state.target_ban_player].banned = true;

                app_state.target_ban_player = std::nullopt;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(2);

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                app_state.target_ban_player = std::nullopt;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        else
        {
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::PopStyleColor(1); // text color

    ImGui::EndChild();
}

void MainLayer::RenderEditSelectedPlayer()
{
    LOG_SCOPE("");
    auto& app_state{m_application->GetApplicationState()};

    if (!app_state.selected_player)
        return;

    auto const idx{*app_state.selected_player};

    ImGui::Begin("Edit Player");

    if (ImGui::InputText("Edit Name", app_state.edits.buffer, sizeof(app_state.edits.buffer)))
    {
        LOG_INFO(
            "Saving player name -> prev{} -> now{}", app_state.players[idx].name, app_state.edits.buffer);
        app_state.players[idx].name = app_state.edits.buffer;
        app_state.save_players_data = true;
        app_state.reorder_players_data = true;
    }

    ImGui::SliderInt("Health", &app_state.edits.health, 0, 100);
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        LOG_INFO(
            "Saving player health -> prev{} -> now{}",
            app_state.players[idx].health,
            app_state.edits.health);
        app_state.players[idx].health = app_state.edits.health;
        app_state.save_players_data = true;
        app_state.reorder_players_data = true;
    }

    if (ImGui::Button("Done"))
    {
        LOG_INFO("Done editing");
        app_state.selected_player = std::nullopt;
    }

    ImGui::End();

    CleanupBanned();
    SavePlayers();
}

void MainLayer::RenderMenuBar()
{
    LOG_SCOPE("");

    auto& app_state{m_application->GetApplicationState()};

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Player", "Ctrl+N"))
            {
                app_state.players.emplace_back("Dummy", 0);
                app_state.sorted_players_indices.push_back(app_state.players.size() - 1);
                app_state.save_players_data = true;
                app_state.reorder_players_data = true;
                SetEditPlayer(app_state.players.size() - 1);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4"))
            { /* Logic */
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            ImGui::MenuItem("Show Player List", nullptr, &app_state.show_players);
            ImGui::EndMenu();
        }

        // --- Right Side Stats ---
        // 1. Calculate how much space the FPS text will take
        float fps = ImGui::GetIO().Framerate;
        char fps_text[32];
        snprintf(fps_text, sizeof(fps_text), "%.1f FPS", fps);

        float text_width = ImGui::CalcTextSize(fps_text).x;

        // 2. Set the cursor to the far right (minus the text width and some padding)
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - text_width - ImGui::GetStyle().ItemSpacing.x);

        // 3. Display it (use a color to make it stand out, maybe your leafGreen?)
        ImGui::TextColored(ImVec4(0.15f, 0.55f, 0.38f, 1.00f), "%s", fps_text);

        ImGui::EndMainMenuBar();
    }
}

void MainLayer::RenderSearchFilter()
{
    LOG_SCOPE("");

    auto& app_state{m_application->GetApplicationState()};

    if (ImGui::InputText("Search Player", app_state.filter.buffer, sizeof(app_state.filter.buffer)))
    {
        app_state.filter.length = strlen(app_state.filter.buffer);

        if (static_cast<bool>(app_state.selected_player))
        {
            if (!NameMatchesSearch(app_state.players[*app_state.selected_player].name))
            {
                app_state.selected_player = std::nullopt;
            }
        }
    }
}

void MainLayer::CleanupBanned()
{
    LOG_SCOPE("");
    auto& app_state{m_application->GetApplicationState()};

    auto const initial_size{app_state.players.size()};
    // 1. If we have a selection, calculate the new index
    if (app_state.selected_player.has_value())
    {
        std::size_t old_idx = *app_state.selected_player;
        std::size_t offset = 0;

        for (std::size_t i = 0; i < old_idx; ++i)
        {
            if (app_state.players[i].banned)
                offset++;
        }

        // 2. If the selected player themselves is banned, clear the selection
        if (app_state.players[old_idx].banned)
        {
            app_state.selected_player = std::nullopt;
        }
        else
        {
            *app_state.selected_player -= offset;
        }
    }

    // 3. Actually remove the players from the vector
    app_state.players.erase(
        std::remove_if(
            app_state.players.begin(),
            app_state.players.end(),
            [](Player const& p) { return p.banned; }),
        app_state.players.end());

    bool const data_changed{app_state.players.size() != initial_size};

    app_state.save_players_data |= data_changed;

    if (data_changed)
    {
        app_state.sorted_players_indices.resize(app_state.players.size());
        std::iota(app_state.sorted_players_indices.begin(), app_state.sorted_players_indices.end(), 0);
        app_state.reorder_players_data = true;
    }
}

void MainLayer::SavePlayers()
{
    LOG_TRACE("");
    auto& app_state{m_application->GetApplicationState()};

    if (!app_state.save_players_data)
    {
        return;
    }
    LOG_SCOPE("");

    std::ofstream file{PlayersApplication::GetPlayersDataPath(), std::ios::trunc};
    LOG_INFO("Saving {} players", app_state.players.size());
    file << app_state.players.size() << '\n';
    std::for_each(app_state.players.begin(), app_state.players.end(), [&](Player const& player) {
        file << player;
    });

    app_state.save_players_data = false;
}

void MainLayer::SetEditPlayer(std::size_t const index)
{
    auto& app_state{m_application->GetApplicationState()};

    app_state.selected_player = index;

    strncpy(
        app_state.edits.buffer, app_state.players[index].name.c_str(), sizeof(app_state.edits.buffer));

    app_state.edits.health = app_state.players[index].health;
}

bool MainLayer::NameMatchesSearch(std::string_view const name) const
{
    auto& app_state{m_application->GetApplicationState()};
    return app_state.filter.length == 0 || name.contains(app_state.filter.buffer);
}

bool MainLayer::IsRenderable(Player const& player) const
{
    return !player.banned && NameMatchesSearch(player.name);
}

} // namespace Graphite::Project::Layers
