//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef WALL_H
#define WALL_H

#include <level/level.h>

#include "raylib.h"

char get_surroundings(unsigned int x, unsigned int y, LoadedLevel *level);
void draw_wall(Vector2 pos, float size, char surroundings);

#endif //WALL_H
