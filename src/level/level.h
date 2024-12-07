//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef LEVEL_H
#define LEVEL_H

#include <vector>

#include "raylib.h"
#include "box2d/id.h"

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

struct LoadedLevel {
    // TODO: Add info about exits
    // LevelInfo info;
    int rows;
    int columns;
    b2WorldId world_id;
    std::vector<LevelTile> tiles;
};

void load_level(LevelPosition position);
void unload_level(LoadedLevel* level);
std::vector<LevelTile> parse_level(const Image *image);
LevelTile get_tile_from_color(Color color);

LoadedLevel *get_current_level();

bool is_colliding(Vector2 pos, LevelTile look_for);
int get_collider_tile_index(Vector2 pos, LevelTile look_for);
void set_tile_at_index(int tile_index, LevelTile tile);

LevelTile get_tile_at(int x, int y, LoadedLevel *level);

#endif //LEVEL_H
