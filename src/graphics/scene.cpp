//
// Created by Timur Turatbekov on 29.11.2024.
//


#include <globals.h>
#include <global_state.h>

#include "text.h"
#include "scenes/level_scene.h"
#include "scenes/menu.h"


void draw_current_scene(const std::unique_ptr<GameState> &game_state) {
    switch (game_state->scene) {
        case MENU_SCENE:
            draw_menu();
            break;
        case LEVEL_SCENE:
            draw_level(game_state->loaded_level);
            draw_game_overlay();
            break;
        default:
            draw_placeholder_scene();
            break;
    }
}

Text placeholder_text = {
    "Work in progress",
    {0.5f, 0.5f},
};

void draw_placeholder_scene() {
    draw_text(placeholder_text);
}

