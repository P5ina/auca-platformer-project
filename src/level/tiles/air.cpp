//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "air.h"

#include <globals.h>

void draw_air(Vector2 pos, float size, Assets* assets) {
    const int frames_count = 4;
    const double frame_time = 2.0;

    double time = GetTime();
    const int frame = (static_cast<int>((time + pos.y * 8) / frame_time)) % frames_count;
    Rectangle source = { frame * 8.0f, 0, 8.0f, 8.0f };
    Rectangle destination = { pos.x, pos.y, size, size };
    DrawTexturePro(assets->images.air_texture, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

