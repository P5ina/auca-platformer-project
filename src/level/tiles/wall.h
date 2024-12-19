//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef WALL_H
#define WALL_H

#include <level/level.h>

#include "raylib.h"

char get_surroundings(Level *level, int x, int y);
char get_surroundings_without_borders(Level *level, Vector2i position);
void draw_wall(Vector2 pos, float size, char surroundings);

#endif //WALL_H
