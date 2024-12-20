//
// Created by Timur Turatbekov on 20.12.2024.
//

#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <global_state.h>
#include <input.h>

void start_dialogue(GameState* game_state, Dialogue* dialogue);
void update_dialogue(GameState* game_state, GameInput* game_input);
void draw_dialogue(GameState* game_state);

#endif //DIALOGUE_H
