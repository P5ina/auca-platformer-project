//
// Created by Timur Turatbekov on 22.12.2024.
//

#include "magnet.h"

#include <graphics/metrics.h>

#include "raylib.h"
#include "raymath.h"

void update_magnet_cursor(MagnetCursor *magnet_cursor, GameInput* game_input) {
    float ui_scale = get_ui_scale();
    float cursor_size = 30.0f * ui_scale;

    float width = get_display_width();
    float height = get_display_height();

    Vector2 cursor_pos = magnet_cursor->position;
    Vector2 mouse_delta = GetMouseDelta();
    float mouse_sens = 1.0f;

    magnet_cursor->is_pull = game_input->magnet_pull;
    magnet_cursor->is_push = game_input->magnet_push;

    cursor_pos = Vector2Add(cursor_pos, Vector2Scale(mouse_delta, mouse_sens));

    if (cursor_pos.x < cursor_size / 2) {
        cursor_pos = { cursor_size / 2, cursor_pos.y };
    }
    if (cursor_pos.x > width - cursor_size / 2) {
        cursor_pos = { width - cursor_size / 2, cursor_pos.y };
    }
    if (cursor_pos.y < cursor_size / 2) {
        cursor_pos = { cursor_pos.x, cursor_size / 2 };
    }
    if (cursor_pos.y > height - cursor_size / 2) {
        cursor_pos = { cursor_pos.x, height - cursor_size / 2 };
    }
    magnet_cursor->position = cursor_pos;
}

void draw_magnet_cursor(MagnetCursor *magnet_cursor, Assets *assets) {
    float ui_scale = get_ui_scale();
    float cursor_size = 26.0f * ui_scale;

    Texture2D texture = assets->images->magnet_cursor;

    float frame = 0;
    if (magnet_cursor->is_pull) {
        frame += 1;
    }
    if (magnet_cursor->is_push) {
        frame += 2;
    }

    Rectangle source = { 8.0f * frame, 0, 8, 8 };
    Rectangle dest = {
        magnet_cursor->position.x - cursor_size / 2, magnet_cursor->position.y - cursor_size / 2,
        cursor_size, cursor_size
    };

    DrawTexturePro(texture, source, dest, { 0, 0 }, 0, WHITE);
}


