#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <nlohmann/json.hpp>

#include "raylib.h"

struct Vector2i {
    int x;
    int y;
};

enum PhysicsLayers {
    PLAYER_LAYER = 0x00000001,
    WALL_LAYER = 0x00000002,
    GHOST_LAYER = 0x00000004,
    COLLECTABLE_LAYER = 0x00000008,
};

float rand_from_to(float from, float to);
float rand_up_to(float to);
Color get_color_from_hex(const std::string& hex);

Vector2i vector2i_add(Vector2i v1, Vector2i v2);

void to_json(nlohmann::json& j, const Vector2& v);
void from_json(const nlohmann::json& j, Vector2& v);

#endif // UTILITIES_H
