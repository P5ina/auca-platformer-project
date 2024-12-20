//
// Created by Timur Turatbekov on 20.12.2024.
//

#include "dialogue.h"

void start_dialogue(GameState* game_state, std::unique_ptr<Dialogue> dialogue) {
    game_state->loaded_dialogue = std::move(dialogue);
}

void update_dialogue(GameState* game_state, GameInput* game_input) {

}

void draw_dialogue(GameState* game_state) {
    if (game_state->loaded_dialogue == nullptr)
        return;

    const float HEIGHT_PERCENT = 0.3f;

}
