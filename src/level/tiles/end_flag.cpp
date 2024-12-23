//
// Created by Timur Turatbekov on 23.12.2024.
//

#include "end_flag.h"

#include <utilities.h>
#include <gameobjects/gameobject_user_data.h>

void draw_end_flag(Vector2 pos, float size, Assets *assets) {
    Rectangle source = { 0, 0, 8.0f, 8.0f };
    Rectangle destination = { pos.x, pos.y, size, size };
    DrawTexturePro(assets->images->end_flag_texture, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}
