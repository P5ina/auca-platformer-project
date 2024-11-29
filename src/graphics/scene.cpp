//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "scene.h"

#include <global_state.h>

#include "text.h"
#include "scenes/menu.h"

void draw_current_scene() {
    switch (global_state.current_scene) {
        case MENU_SCENE:
            draw_menu();
            break;
        case LEVEL_SCENE:
            draw_level();
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

