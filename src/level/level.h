//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef LEVEL_H
#define LEVEL_H

#include <global_state.h>
#include <utilities.h>
#include <vector>

#include "raylib.h"
#include "box2d/id.h"

const std::string AIR_COLOR = "#000000";
const std::string WALL_COLOR = "#ffffff";
const std::string PLAYER_SPAWN_COLOR = "#6abe30";
const std::string COIN_COLOR = "#fbf236";

struct LevelPosition {
    int x, y, z;
};

void load_level(std::unique_ptr<GameState> &game_state, LevelPosition position);
void unload_level(std::unique_ptr<GameState> &game_state);
std::unique_ptr<Level> parse_level(const Image *image);
LevelTile create_level_tile(Color color, Vector2i position);

void set_tile(std::unique_ptr<Level> &level, LevelTile tile, Vector2i position);
void set_tile(std::unique_ptr<Level> &level, LevelTile tile, int tile_index);
std::optional<LevelTile> get_tile(Level *level, Vector2i position);
std::optional<LevelTile> get_tile(Level *level, int tile_index);
std::optional<int> get_tile_index(Level *level, Vector2i position);

#endif //LEVEL_H
