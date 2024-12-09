//
// Created by Timur Turatbekov on 29.11.2024.
//


#include "scene.h"

#include "text.h"
#include "scenes/level_scene.h"
#include "scenes/menu.h"
#include <global_state.h>


void draw_current_scene(const std::unique_ptr<GameState> &game_state) {
    switch (game_state->scene) {
        case Scene::MENU_SCENE:
            draw_menu();
            break;
        case Scene::LEVEL_SCENE:
            draw_level(game_state->loaded_level);
            draw_game_overlay();
            break;
        default:
            draw_placeholder_scene();
            break;
    }
}


void draw_placeholder_scene() {
    const Text placeholder_text = {
        "Work in progress",
        {0.5f, 0.5f},
    };
    draw_text(placeholder_text);
}

