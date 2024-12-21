//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <assets.h>
#include <memory>
#include <utilities.h>
#include <vector>
#include <graphics/scene.h>
#include <graphics/scenes/dialogue.h>

#include "box2d/id.h"

struct LevelPosition {
    int x, y, z;
};

struct KnowledgeBook {
    LevelPosition level_position;
    Vector2 position;
    std::string tip_text;
    bool isCollected = false;
};

struct Player {
    b2BodyId body_id;
    float jump_timer;
    bool movement_locked;
    bool is_facing_left;
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
    std::vector<KnowledgeBook> books;
};

struct MainMenuState {
    float elapsed_time = 0.0f;
    bool fade_in = true;
    std::vector<int> title_char_appearing_order;
    int menu_option = 0;
};

struct GameState {
    Scene scene = Scene::MENU_SCENE;
    std::unique_ptr<Level> loaded_level;
    std::unique_ptr<Dialogue> loaded_dialogue;
    std::unique_ptr<Player> player;
    std::unique_ptr<Assets> assets;
    Vector2i last_resolution;
    bool debug_mode = false;
    MainMenuState main_menu_state;
};

#endif //GLOBAL_STATE_H
