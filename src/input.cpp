//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "input.h"

#include "raylib.h"

GameInput read_game_input() {
    GameInput game_input = {
        0.0f,
        false
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

    return game_input;
}

