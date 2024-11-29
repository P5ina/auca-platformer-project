//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "level.h"

#include <format>
#include <array>
#include <global_state.h>
#include <string>
#include <utilities.h>

const int LEVELS_NUMBER = 1;

std::array<LevelPosition, LEVELS_NUMBER> LEVEL_POSITIONS = {
    LevelPosition { 0, 0, 0}
};

std::array<Level, LEVELS_NUMBER> LEVELS;

void load_levels() {
    const std::string filepath_prefix = ASSETS_PATH"images/levels/";

    for (int i = 0; i < LEVELS_NUMBER; i++) {
        auto [x, y, z] = LEVEL_POSITIONS[i];
        std::string filepath = std::format("{}level_{}_{}_{}.png", filepath_prefix, x, y, z);
        Image level_image = LoadImage(filepath.c_str());
        LEVELS[i] = parse_level(&level_image, LEVEL_POSITIONS[i]);
    }
}

Level parse_level(Image *image, LevelPosition position) {
    int width = image->width;
    int height = image->height;

    Level level = Level {
        position,
        height,
        width,
        std::vector<LevelTile>(height * width)
    };

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color tile_color = GetImageColor(*image, x, y);
            LevelTile tile = get_tile_from_color(tile_color);
            level.tiles[y * width + x] = tile;
        }
    }

    return level;
}

// Could be done using dictionaries
LevelTile get_tile_from_color(Color color) {
    if (ColorIsEqual(color, get_color_from_hex(AIR_COLOR))) {
        return AIR;
    }
    if (ColorIsEqual(color, get_color_from_hex(WALL_COLOR))) {
        return WALL;
    }
    if (ColorIsEqual(color, get_color_from_hex(PLAYER_SPAWN_COLOR))) {
        return PLAYER_SPAWN;
    }
    if (ColorIsEqual(color, get_color_from_hex(COIN_COLOR))) {
        return COIN;
    }
    if (ColorIsEqual(color, get_color_from_hex(EXIT_COLOR))) {
        return EXIT;
    }
    return AIR;
}

Level get_current_level() {
    return LEVELS[global_state.current_level];
}

bool is_colliding(Vector2 pos, LevelTile look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    auto level = get_current_level();

    for (size_t row = 0; row < level.rows; ++row) {
        for (size_t column = 0; column < level.columns; ++column) {
            if (level.tiles[row * level.columns + column] == look_for) {
                Rectangle block_hitbox = {static_cast<float>(column), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<LevelTile>::iterator get_collider(Vector2 pos, LevelTile look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    auto level = get_current_level();

    for (size_t row = 0; row < level.rows; ++row) {
        for (size_t column = 0; column < level.columns; ++column) {
            if (level.tiles[row * level.columns + column] == look_for) {
                Rectangle block_hitbox = {static_cast<float>(column), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return level.tiles.begin() + row * level.columns + column;
                }
            }
        }
    }

    return level.tiles.begin() + static_cast<int>(roundf(pos.y) * level.columns + roundf(pos.x));
}

void load_level(LevelPosition position) {
    int level_index = 0;
    for (int i = 0; i < LEVELS_NUMBER; i++) {
        if (LEVELS[i].position.x == position.x &&
            LEVELS[i].position.y == position.y &&
            LEVELS[i].position.z == position.z) {
            level_index = i;
            break;
        }
    }

    global_state.current_level = level_index;
    spawn_player();
    derive_graphics_metrics_from_loaded_level();
}
