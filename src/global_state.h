//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <memory>
#include <optional>
#include <utilities.h>
#include <vector>
#include <graphics/scene.h>

#include "box2d/id.h"

struct Player {
    b2BodyId body_id;
    float jump_timer;
};

enum class LevelTileType {
    AIR,
    WALL,
    PLAYER_SPAWN,
    COIN
};

struct LevelTile {
    Vector2i position;
    LevelTileType type;
};

struct Level {
    int rows;
    int columns;
    b2WorldId world_id;
    std::vector<LevelTile> tiles;
    std::vector<b2BodyId> wall_bodies;
};

struct GameState {
    Scene scene = Scene::MENU_SCENE;
    std::unique_ptr<Level> loaded_level;
    std::unique_ptr<Player> player;
    bool debug_mode;
    // TODO: Assets
    // Assets assets;
};

#endif //GLOBAL_STATE_H
