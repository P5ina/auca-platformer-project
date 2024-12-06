//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "wall.h"

#include <globals.h>

char get_surroundings(unsigned int x, unsigned int y, LoadedLevel *level) {
    bool top = get_tile_at(x, y - 1, level) == LevelTile::WALL;
    bool right = get_tile_at(x + 1, y, level) == LevelTile::WALL;
    bool bottom = get_tile_at(x, y + 1, level) == LevelTile::WALL;
    bool left = get_tile_at(x - 1, y, level) == LevelTile::WALL;

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

