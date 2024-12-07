//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "player.h"

#include <globals.h>

void spawn_player(std::unique_ptr<GameState> &game_state) {
    const std::unique_ptr<LoadedLevel> &level = game_state->loaded_level;

    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            LevelTile tile = level->tiles[row * level->columns + column];
            if (tile == PLAYER_SPAWN) {
                player_pos.x = static_cast<float>(column);
                player_pos.y = static_cast<float>(row);
                return;
            }
        }
    }
}

void move_player_horizontally(std::unique_ptr<GameState> &game_state, float delta) {
    float next_x = player_pos.x + delta;
    if (!is_colliding(game_state->loaded_level, {next_x, player_pos.y}, WALL)) {
        player_pos.x = next_x;
    } else {
        player_pos.x = roundf(player_pos.x);
    }
}

void update_player(std::unique_ptr<GameState> &game_state) {
    player_pos.y += player_y_velocity;
    player_y_velocity += GRAVITY_FORCE;

    is_player_on_ground = is_colliding(game_state->loaded_level, { player_pos.x, player_pos.y + 0.1f }, WALL);
    if (is_player_on_ground) {
        player_y_velocity = 0;
        player_pos.y = roundf(player_pos.y);
    }

    if (is_colliding(game_state->loaded_level, player_pos, COIN)) {
        int index = get_collider_tile_index(game_state->loaded_level, player_pos, COIN);
        set_tile_at_index(game_state->loaded_level, index, AIR);
        player_score += 10;
        PlaySound(coin_sound);
    }
    if (is_colliding(game_state->loaded_level, player_pos, EXIT)) {
        // TODO
    }
}