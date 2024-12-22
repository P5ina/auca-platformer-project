//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <assets.h>
#include <memory>
#include <utilities.h>
#include <vector>
#include <gameobjects/magnet.h>
#include <graphics/scene.h>
#include <graphics/scenes/dialogue.h>
#include <graphics/scenes/pause.h>
#include <level/level.h>

#include "box2d/id.h"

constexpr Vector2 PLAYER_SPAWN = { 8, 8 };

struct Player {
    b2BodyId body_id;
    b2BodyId attached_to_body_id;
    MagnetCursor magnet_cursor;
    float jump_timer;
    bool movement_locked;
    bool is_grounded;
    bool is_facing_left;
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
    bool close = false;
    MainMenuState main_menu_state;
    PauseMenuState pause_menu_state;
};

void update_game(std::unique_ptr<GameState> &game_state, float delta);

#endif //GLOBAL_STATE_H
