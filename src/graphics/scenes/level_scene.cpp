//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "level_scene.h"

#include <globals.h>
#include <graphics/text.h>
#include <level/tiles/air.h>
#include <level/tiles/wall.h>

void draw_game_overlay() {
    Text score = {
        "Score " + std::to_string(player_score),
        { 0.50f, 0.05f },
        48.0f
    };
    Text score_shadow = {
        "Score " + std::to_string(player_score),
        { 0.503f, 0.055f },
        48.0f,
        GRAY
    };

    draw_text(score_shadow);
    draw_text(score);
}

void draw_level(std::unique_ptr<LoadedLevel> &level) {
    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            Vector2 pos = {
                shift_to_center.x + static_cast<float>(column) * cell_size,
                shift_to_center.y + static_cast<float>(row) * cell_size
            };

            LevelTile tile = level->tiles[row * level->columns + column];
            // The first image layer
            switch (tile.type) {
                case LevelTileType::AIR:
                case LevelTileType::PLAYER_SPAWN:
                case LevelTileType::COIN:
                    draw_air(pos, cell_size);
                break;
                case LevelTileType::WALL:
                    char surroundings = get_surroundings(level, column, row);
                draw_wall(pos, cell_size, surroundings);
                break;
            }
            // The second image layer
            switch (tile.type) {
                case LevelTileType::COIN:
                    draw_sprite(coin_sprite, pos, cell_size);
                break;
                default:
                    break;
            }
        }
    }

    draw_player();
}

void draw_player() {
    // Vector2 pos = {
    //     shift_to_center.x + player_pos.x * cell_size,
    //     shift_to_center.y + player_pos.y * cell_size
    // };
    //
    // draw_sprite(player_sprite, pos, cell_size);
}
