//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "input.h"

#include "raylib.h"

GameInput read_game_input() {
    auto game_input = GameInput {
        .horizontal_movement = 0.0f,
        .select_up_pressed = false,
        .select_down_pressed = false,
        .select_pressed = false,
        .jump = false,
        .toggle_debug_mode = false,
        .toggle_full_screen = false,
        .skip_dialogue_line = false,
    };

    if (IsKeyDown(KeyboardKey::KEY_D) | IsKeyDown(KeyboardKey::KEY_RIGHT)) {
        game_input.horizontal_movement += 1.0f;
    }
    if (IsKeyDown(KeyboardKey::KEY_A) | IsKeyDown(KeyboardKey::KEY_LEFT)) {
        game_input.horizontal_movement -= 1.0f;
    }

    if (IsKeyPressed(KeyboardKey::KEY_W) | IsKeyPressed(KeyboardKey::KEY_UP)) {
        game_input.select_up_pressed = true;
    }
    if (IsKeyPressed(KeyboardKey::KEY_S) | IsKeyPressed(KeyboardKey::KEY_DOWN)) {
        game_input.select_down_pressed = true;
    }
    if (IsKeyPressed(KeyboardKey::KEY_SPACE) | IsKeyPressed(KeyboardKey::KEY_ENTER)) {
        game_input.select_pressed = true;
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

    if (IsKeyPressed(KeyboardKey::KEY_ESCAPE)) {
        game_input.pause_pressed = true;
    }

    if (IsMouseButtonPressed(0)) {
        game_input.skip_dialogue_line = true;
    }

    return game_input;
}

