/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file AppState.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief AppState.
///

#pragma once

#include <cstdint>
#include <istream>
#include <optional>
#include <ostream>
#include <vector>

namespace Graphite::Project {

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

struct AppState
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
    std::optional<std::size_t> target_ban_player{std::nullopt};
    bool open_ban_modal{false};
    std::vector<Player> players{};
    std::vector<std::size_t> sorted_players_indices{};
};

} // namespace Graphite::Project
