//
// Created by Timur Turatbekov on 22.12.2024.
//

#ifndef MAGNET_H
#define MAGNET_H
#include <assets.h>

#include "raylib.h"
#include "box2d/box2d.h"

constexpr float MAX_MAGNET_DISTANCE = 3;

struct MagnetCursor {
    Vector2 position;
    b2BodyId attached_to;
    Vector2 delta;
    b2Vec2 target_pos;
    bool is_player_moving;
    bool is_pull;
    bool is_push;
};

void draw_magnet_cursor(MagnetCursor* magnet_cursor, Assets* assets);
void draw_magnet_effect(MagnetCursor *magnet_cursor, Texture2D texture);

#endif //MAGNET_H
