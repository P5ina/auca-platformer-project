//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <memory>
#include <utilities.h>
#include <vector>
#include <graphics/scene.h>

#include "box2d/id.h"

struct Player {
    b2BodyId body_id;
    float jump_timer;
    bool movement_locked;
};

enum class LevelTileType {
    AIR,
    WALL
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

struct MainMenuState {
    float elapsed_time = 0.0f;
    bool fade_in = true;
    std::vector<int> title_char_appearing_order;
};

struct GameState {
    Scene scene = Scene::MENU_SCENE;
    std::unique_ptr<Level> loaded_level;
    std::unique_ptr<Player> player;
    bool debug_mode = false;
    MainMenuState main_menu_state;
    // TODO: Assets
    // Assets assets;
};

#endif //GLOBAL_STATE_H
