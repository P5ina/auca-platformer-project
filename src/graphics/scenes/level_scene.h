//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H
#include <memory>
#include <level/level.h>

void draw_player(std::unique_ptr<GameState> &game_state);
void draw_level(std::unique_ptr<GameState> &game_state);
void draw_game_overlay();

#endif //LEVEL_SCENE_H
