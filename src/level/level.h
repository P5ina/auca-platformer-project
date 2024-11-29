//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "raylib.h"

enum LevelTile {
    AIR,
    WALL,
    PLAYER_SPAWN,
    COIN,
    EXIT
};

const std::string AIR_COLOR = "#000000";
const std::string WALL_COLOR = "#ffffff";
const std::string PLAYER_SPAWN_COLOR = "#6abe30";
const std::string COIN_COLOR = "#fbf236";
const std::string EXIT_COLOR = "#76428a";

struct LevelPosition {
    int x, y, z;
};

struct Level {
    LevelPosition position;
    int rows;
    int columns;
    std::vector<LevelTile> tiles;
};

void load_levels();
Level parse_level(Image *image, LevelPosition position);
LevelTile get_tile_from_color(Color color);

Level get_current_level();

bool is_colliding(Vector2 pos, LevelTile look_for);
std::vector<LevelTile>::iterator get_collider(Vector2 pos, LevelTile look_for);

void load_level(LevelPosition position);

#endif //LEVEL_H
