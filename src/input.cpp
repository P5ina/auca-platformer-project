//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "input.h"

#include "raylib.h"

GameInput read_game_input() {
    GameInput game_input = GameInput {
        .horizontal_movement = 0.0f,
        .jump = false,
        .toggle_debug_mode = false,
        .toggle_full_screen = false
    };

    if (IsKeyDown(KeyboardKey::KEY_D) | IsKeyDown(KeyboardKey::KEY_RIGHT)) {
        game_input.horizontal_movement += 1.0f;
    }
    if (IsKeyDown(KeyboardKey::KEY_A) | IsKeyDown(KeyboardKey::KEY_LEFT)) {
        game_input.horizontal_movement -= 1.0f;
    }

    if (IsKeyDown(KeyboardKey::KEY_SPACE)) {
        game_input.jump = true;
    }

    if (IsKeyPressed(KeyboardKey::KEY_F3)) {
        game_input.toggle_debug_mode = true;
    }

    if (IsKeyPressed(KeyboardKey::KEY_F5)) {
        game_input.toggle_full_screen = true;
    }

    return game_input;
}

