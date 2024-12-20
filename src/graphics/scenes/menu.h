//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef MENU_H
#define MENU_H
#include <global_state.h>
#include <input.h>

void init_main_menu(GameState *game_state);
void update_menu(GameState *game_state, GameInput *game_input);

void draw_menu(GameState *game_state);
void draw_game_title(GameState *game_state);
void draw_start_message(GameState *game_state);

#endif //MENU_H
