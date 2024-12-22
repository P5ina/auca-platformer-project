//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef LEVEL_SCENE_H
#define LEVEL_SCENE_H
#include <global_state.h>
#include <gameobjects/magnetic_box.h>
#include <level/level.h>

void draw_level(Level* level, Assets* assets);
void draw_level_debug_overlay(Level* level);

void draw_player(Level* level, Player* player, Assets* assets);
void draw_player_debug_overlay(Level* level, Player* player);

void draw_book(Level* level, KnowledgeBook* book, Assets* assets);
void draw_book_debug_overlay(Level* level, KnowledgeBook* book);

void draw_magnetic_box(Level* level, MagneticBox* magnetic_box, Assets* assets);

#endif //LEVEL_SCENE_H
