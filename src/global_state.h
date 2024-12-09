//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <memory>
#include <optional>
#include <vector>
#include <graphics/scene.h>

#include "box2d/id.h"

struct Player {
    b2BodyId body_id;
};

enum class LevelTileType {
    AIR,
    WALL,
    PLAYER_SPAWN,
    COIN
};

struct LevelTile {
    LevelTileType type;
    std::optional<b2BodyId> body_id;
};

struct LoadedLevel {
    int rows;
    int columns;
    b2WorldId world_id;
    std::vector<LevelTile> tiles;
};

struct GameState {
    Scene scene = Scene::MENU_SCENE;
    std::unique_ptr<LoadedLevel> loaded_level;
    std::unique_ptr<Player> player;
    // TODO: Assets
    // Assets assets;
};

#endif //GLOBAL_STATE_H
