//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "wall.h"

#include <globals.h>

char get_surroundings(Level *level, int x, int y) {
    bool top = true;
    bool right = true;
    bool bottom = true;
    bool left = true;

    if (auto top_tile = get_tile(level, { x, y - 1} )) {
        top = top_tile->type == LevelTileType::WALL;
    }
    if (auto right_tile = get_tile(level, { x + 1, y} )) {
        right = right_tile->type == LevelTileType::WALL;
    }
    if (auto bottom_tile = get_tile(level, { x, y + 1} )) {
        bottom = bottom_tile->type == LevelTileType::WALL;
    }
    if (auto left_tile = get_tile(level, { x - 1, y} )) {
        left = left_tile->type == LevelTileType::WALL;
    }

    return top + (right << 1) + (bottom << 2) + (left << 3);
}

char get_surroundings_without_borders(Level *level, Vector2i position) {
    int x = position.x;
    int y = position.y;
    bool top = false;
    bool right = false;
    bool bottom = false;
    bool left = false;

    if (auto top_tile = get_tile(level, { x, y - 1} )) {
        top = top_tile->type == LevelTileType::WALL;
    }
    if (auto right_tile = get_tile(level, { x + 1, y} )) {
        right = right_tile->type == LevelTileType::WALL;
    }
    if (auto bottom_tile = get_tile(level, { x, y + 1} )) {
        bottom = bottom_tile->type == LevelTileType::WALL;
    }
    if (auto left_tile = get_tile(level, { x - 1, y} )) {
        left = left_tile->type == LevelTileType::WALL;
    }

    return top + (right << 1) + (bottom << 2) + (left << 3);
}

void draw_wall(Vector2 pos, float size, char surroundings) {
    const unsigned int surroundings_mappings[16] = {
        0, 12, 1, 13, 4, 8, 5, 9, 3, 15, 2, 14, 7, 11, 6, 10
    };

    unsigned int source_index = surroundings_mappings[surroundings];
    float source_x = static_cast<float>(source_index % 4) * 8.0f;
    float source_y = static_cast<float>(source_index / 4) * 8.0f;
    Rectangle source = { source_x, source_y, 8.0f, 8.0f };
    Rectangle destination = { pos.x, pos.y, size, size };
    DrawTexturePro(wall_image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

