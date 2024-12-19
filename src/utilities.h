#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "raylib.h"

struct Vector2i {
    int x;
    int y;
};

enum class PhysicsCategories {
    PLAYER = 0x00000001,
    WALL = 0x00000002,
    GHOST = 0x00000004,
};

float rand_from_to(float from, float to);
float rand_up_to(float to);
Color get_color_from_hex(const std::string& hex);

Vector2i vector2i_add(Vector2i v1, Vector2i v2);

#endif // UTILITIES_H
