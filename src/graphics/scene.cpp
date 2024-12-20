//
// Created by Timur Turatbekov on 29.11.2024.
//


#include "scene.h"

#include "text.h"
#include "scenes/level_scene.h"
#include "scenes/menu.h"
#include <global_state.h>


void draw_current_scene(std::unique_ptr<GameState> &game_state) {
    switch (game_state->scene) {
        case Scene::MENU_SCENE: {
            // draw_level(game_state->loaded_level.get(), game_state->assets.get());
            draw_player(game_state->player.get(), game_state->assets.get());
            draw_menu(game_state.get());
            break;
        }
        case Scene::LEVEL_SCENE: {
            draw_level(game_state->loaded_level.get(), game_state->assets.get());
            draw_player(game_state->player.get(), game_state->assets.get());
            // draw_game_overlay();
            break;
        }
        default: {
            draw_placeholder_scene();
            break;
        }
    }
}

void draw_level_scene(GameState *game_state) {
    draw_level(game_state->loaded_level.get(), game_state->assets.get());
    if (game_state->debug_mode) {
        draw_level_debug_overlay(game_state->loaded_level.get());
    }
}


void draw_placeholder_scene() {
    const Text placeholder_text = {
        "Work in progress",
        {0.5f, 0.5f},
    };
    draw_text(placeholder_text);
}

