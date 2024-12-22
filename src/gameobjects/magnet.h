//
// Created by Timur Turatbekov on 22.12.2024.
//

#ifndef MAGNET_H
#define MAGNET_H
#include <assets.h>
#include <input.h>
#include <level/level.h>

#include "raylib.h"
#include "box2d/box2d.h"

struct MagnetCursor {
    Vector2 position;
    b2BodyId attached_to;
    bool is_player_moving;
    bool is_pull;
    bool is_push;
};

void update_magnet_cursor(MagnetCursor* magnet_cursor, Level* level, GameInput* game_input);
void draw_magnet_cursor(MagnetCursor* magnet_cursor, Assets* assets);
void draw_magnet_effect(MagnetCursor *magnet_cursor, Texture2D texture);

#endif //MAGNET_H
