//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H
#include <level/level.h>

void draw_player(Player* player, Assets* assets);
void draw_level(Level* level, Assets* assets);
void draw_level_debug_overlay(Level* level);
void draw_game_overlay();

#endif //LEVEL_SCENE_H
