//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "level.h"

#include <format>
#include <global_state.h>
#include <string>
#include <utilities.h>

LoadedLevel current_loaded_level;

void load_level(LevelPosition position) {
    const std::string filepath_prefix = ASSETS_PATH"images/levels/";

    auto [x, y, z] = position;
    std::string filepath = std::format("{}level_{}_{}_{}.png", filepath_prefix, x, y, z);
    Image level_image = LoadImage(filepath.c_str());

    LoadedLevel new_level = LoadedLevel {
        level_image.height,
        level_image.width,
        parse_level(&level_image)
    };

    current_loaded_level = new_level;

    UnloadImage(level_image);

    spawn_player();
    derive_graphics_metrics_from_loaded_level();
}

std::vector<LevelTile> parse_level(const Image *image) {
    int width = image->width;
    int height = image->height;

    std::vector<LevelTile> tiles(height * width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color tile_color = GetImageColor(*image, x, y);
            LevelTile tile = get_tile_from_color(tile_color);
            tiles[y * width + x] = tile;
        }
    }

    return tiles;
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

LoadedLevel *get_current_level() {
    return &current_loaded_level;
}

bool is_colliding(Vector2 pos, LevelTile look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    auto level = get_current_level();

    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            if (level->tiles[row * level->columns + column] == look_for) {
                Rectangle block_hitbox = {static_cast<float>(column), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return true;
                }
            }
        }
    }
    return false;
}

int get_collider_tile_index(Vector2 pos, LevelTile look_for) {
    Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};
    auto level = get_current_level();

    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            if (level->tiles[row * level->columns + column] == look_for) {
                Rectangle block_hitbox = {static_cast<float>(column), static_cast<float>(row), 1.0f, 1.0f};
                if (CheckCollisionRecs(player_hitbox, block_hitbox)) {
                    return row * level->columns + column;
                }
            }
        }
    }

    return static_cast<int>(roundf(pos.y) * level->columns + roundf(pos.x));
}

void set_tile_at_index(int tile_index, LevelTile tile) {
    auto level = get_current_level();
    level->tiles[tile_index] = tile;
}

LevelTile get_tile_at(int x, int y, LoadedLevel *level) {
    if (x < 0 || x >= level->columns ||
        y < 0 || y >= level->rows) {
        return LevelTile::AIR;
    }
    return level->tiles[x + y * level->columns];
}

