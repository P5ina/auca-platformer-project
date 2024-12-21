//
// Created by Timur Turatbekov on 20.12.2024.
//

#include "dialogue.h"

#include <assets.h>
#include <globals.h>
#include <graphics/metrics.h>

#include "raylib.h"
#include "raymath.h"

void update_dialogue(Dialogue *dialogue, GameInput *game_input, Assets *assets) {
    if (dialogue == nullptr)
        return;

    const float CHAR_DURATION = 0.03f;
    const float CHAR_DOT_DURATION = 0.3f;
    const int SOUND_PER_CHAR = 3;

    const float PITCH_DEVIATION = 0.1f;

    dialogue->last_update += GetFrameTime();

    if (dialogue->lines[dialogue->current_line].length() > dialogue->current_line_progress) {
        bool play_sound = false;
        char current_char = dialogue->lines[dialogue->current_line][dialogue->current_line_progress];
        if (current_char == '.' || current_char == '!' || current_char == '?') {
            if (dialogue->last_update > CHAR_DOT_DURATION) {
                dialogue->last_update = 0;
                dialogue->current_line_progress++;
                play_sound = false;
            }
        } else {
            if (dialogue->last_update > CHAR_DURATION) {
                dialogue->last_update = 0;
                dialogue->current_line_progress++;
                play_sound = dialogue->current_line_progress % SOUND_PER_CHAR == 0;
            }
        }

        if (play_sound) {
            int sound_index = (dialogue->current_line_progress / SOUND_PER_CHAR) %
                static_cast<int>(assets->sounds->dialogue_sound_pool.size());

            float pitch = 1.0f + rand_from_to(-PITCH_DEVIATION, PITCH_DEVIATION);
            SetSoundPitch(assets->sounds->dialogue_sound_pool[sound_index], pitch);
            PlaySound(assets->sounds->dialogue_sound_pool[sound_index]);
        }
    }
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
    float dest_height = 110.0f * ui_scale;

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

    // RENDER TEXT
    float font_size = 12.0f * ui_scale;
    int max_line_length = 70;
    Vector2 text_position = {x_offset + corner_size, y_offset + corner_size};

    std::string left_text = dialogue->lines[dialogue->current_line].substr(0, dialogue->current_line_progress + 1);
    while (left_text.length() > 0) {
        std::string text = left_text;
        if (text.length() > max_line_length) {
            text = text.substr(0, max_line_length);
            left_text = left_text.substr(max_line_length);
        } else {
            left_text = "";
        }

        DrawTextPro(menu_font, text.c_str(), text_position, {0, 0}, 0.0f, font_size,
                    1.0f, WHITE
        );

        text_position = Vector2Add(text_position, {0, font_size + 5.0f * ui_scale});
    }
}
