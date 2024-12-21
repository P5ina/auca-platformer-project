//
// Created by Timur Turatbekov on 20.12.2024.
//

#include "dialogue.h"

#include <assets.h>
#include <graphics/metrics.h>

#include "raylib.h"

void update_dialogue(Dialogue *dialogue, GameInput *game_input) {
    const float CHAR_DURATION = 0.1f;
    const float CHAR_DOT_DURATION = 0.3f;
    const int SOUND_PER_CHAR = 3;

    dialogue->last_update += GetFrameTime();
}

void draw_dialogue(Dialogue *dialogue, Assets *assets) {
    if (dialogue == nullptr)
        return;

    float ui_scale = get_ui_scale();

    Texture2D texture = *assets->images->dialogue_panel_texture;
    float x_offset = 60.0f * ui_scale;
    float y_offset = 40.0f * ui_scale;
    const float corner_size = 32.0f * ui_scale;

    Rectangle top_left_source = {0.0f, 0.0f, 8.0f, 8.0f};
    Rectangle top_source = {8.0f, 0.0f, 16.0f, 8.0f};
    Rectangle top_right_source = {24.0f, 0.0f, 8.0f, 8.0f};
    Rectangle left_source = {0.0f, 8.0f, 8.0f, 16.0f};
    Rectangle center_source = {8.0f, 8.0f, 16.0f, 16.0f};
    Rectangle right_source = {24.0f, 8.0f, 8.0f, 16.0f};
    Rectangle bottom_left_source = {0.0f, 24.0f, 8.0f, 8.0f};
    Rectangle bottom_source = {8.0f, 24.0f, 16.0f, 8.0f};
    Rectangle bottom_right_source = {24.0f, 24.0f, 8.0f, 8.0f};

    float dest_width = static_cast<float>(get_display_width()) - 2 * x_offset;
    float dest_height = 100.0f * ui_scale;

    auto top_left_dest = Rectangle{x_offset, y_offset, corner_size, corner_size};
    auto top_dest = Rectangle{x_offset + corner_size, y_offset, dest_width - 2 * corner_size, corner_size};
    auto top_right_dest = Rectangle{x_offset + dest_width - corner_size, y_offset, corner_size, corner_size};
    auto left_dest = Rectangle{x_offset, y_offset + corner_size, corner_size, dest_height - 2 * corner_size};
    auto center_dest = Rectangle{
        x_offset + corner_size, y_offset + corner_size, dest_width - 2 * corner_size, dest_height - 2 * corner_size
    };
    auto right_dest = Rectangle{
        x_offset + dest_width - corner_size, y_offset + corner_size, corner_size, dest_height - 2 * corner_size
    };
    auto bottom_left_dest = Rectangle{x_offset, y_offset + dest_height - corner_size, corner_size, corner_size};
    auto bottom_dest = Rectangle{
        x_offset + corner_size, y_offset + dest_height - corner_size, dest_width - 2 * corner_size, corner_size
    };
    auto bottom_right_dest = Rectangle{
        x_offset + dest_width - corner_size, y_offset + dest_height - corner_size, corner_size, corner_size
    };

    Color color = Fade(WHITE, 0.8f);
    DrawTexturePro(texture, top_left_source, top_left_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, top_source, top_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, top_right_source, top_right_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, left_source, left_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, center_source, center_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, right_source, right_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, bottom_left_source, bottom_left_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, bottom_source, bottom_dest, {0.0f, 0.0f}, 0.0f, color);
    DrawTexturePro(texture, bottom_right_source, bottom_right_dest, {0.0f, 0.0f}, 0.0f, color);
}
