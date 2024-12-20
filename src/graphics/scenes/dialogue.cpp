//
// Created by Timur Turatbekov on 20.12.2024.
//

#include "dialogue.h"

#include <assets.h>

#include "raylib.h"

void update_dialogue(Dialogue *dialogue, GameInput *game_input) {
    const float CHAR_DURATION = 0.1f;
    const float CHAR_DOT_DURATION = 0.3f;
    const int SOUND_PER_CHAR = 3;

    dialogue->last_update += GetFrameTime();
}

void draw_dialogue(Dialogue *dialogue, Assets* assets) {
    // if (dialogue == nullptr)
    //     return;

    Texture2D texture = *assets->images->dialogue_panel_texture;
    float x_offset = 120.0f;
    float y_offset = 80.0f;

    const float HEIGHT_PERCENT = 0.3f;
    auto nine_patch_info = NPatchInfo {
        .layout = NPatchLayout::NPATCH_NINE_PATCH,
        .source = Rectangle { 0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height) },
        .top = 8 * 8,
        .bottom = 8 * 8,
        .left = 8 * 8,
        .right = 8 * 8,
    };
    auto dest = Rectangle {
        x_offset, y_offset,
        static_cast<float>(GetScreenWidth()) - 2 * x_offset, static_cast<float>(GetScreenHeight()) * HEIGHT_PERCENT };
    DrawTextureNPatch(texture, nine_patch_info, dest, { 0.0f, 0.0f }, 0.0f, Fade(WHITE, 0.8f));
}
