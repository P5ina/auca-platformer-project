//
// Created by Timur Turatbekov on 22.12.2024.
//

#include "magnet.h"

#include <iostream>
#include <graphics/metrics.h>

#include "gameobject_user_data.h"
#include "raylib.h"
#include "raymath.h"

void draw_magnet_cursor(MagnetCursor *magnet_cursor, Assets *assets) {
    float ui_scale = get_ui_scale();
    float cursor_size = 26.0f * ui_scale;

    Texture2D texture = assets->images->magnet_cursor;

    float cursor_frame = 0;
    if (magnet_cursor->is_pull) {
        cursor_frame += 1;
    }
    if (magnet_cursor->is_push) {
        cursor_frame += 2;
    }

    Rectangle source = {8.0f * cursor_frame, 0, 8, 8};
    Rectangle dest = {
        magnet_cursor->position.x - cursor_size / 2, magnet_cursor->position.y - cursor_size / 2,
        cursor_size, cursor_size
    };

    if (magnet_cursor->is_pull) {
        draw_magnet_effect(magnet_cursor, assets->images->magnet_pulling_texture);
    }
    if (magnet_cursor->is_push) {
        draw_magnet_effect(magnet_cursor, assets->images->magnet_pushing_texture);
    }

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
}

void draw_magnet_effect(MagnetCursor *magnet_cursor, Texture2D texture) {
    int frame = static_cast<int>(GetTime() / 0.1) % 4;
    float effect_size = 95.0f * get_ui_scale();

    Rectangle source = {static_cast<float>(frame) * 16, 0, 16, 16};
    Rectangle dest = {
        magnet_cursor->position.x - effect_size / 2, magnet_cursor->position.y - effect_size / 2,
        effect_size, effect_size
    };

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
}
