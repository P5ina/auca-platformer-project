//
// Created by Timur Turatbekov on 23.12.2024.
//

#include "end.h"

#include <globals.h>
#include <graphics/metrics.h>

#include "raylib.h"
#include "raymath.h"

void draw_end() {
    float ui_scale = get_ui_scale();
    float title_font_size = 32.0f * ui_scale;
    Vector2 title_offset = Vector2Scale({ 0.0f, -40.0f }, ui_scale);
    std::string title_text = "THE END";
    Vector2 title_size = MeasureTextEx(menu_font, title_text.c_str(), title_font_size, 1.0f);
    Vector2 title_position = {
        static_cast<float>(get_display_width()) / 2 - title_size.x / 2 + title_offset.x,
        static_cast<float>(get_display_height()) / 2 - title_size.y / 2 + title_offset.y
    };

    float hint_font_size = 10.0f * ui_scale;
    Vector2 hint_offset = Vector2Scale({ 0.0f, 20.0f }, ui_scale);
    std::string hint_text = "Thanks for playing the game. It's kinda short.";
    Vector2 hint_size = MeasureTextEx(menu_font, hint_text.c_str(), hint_font_size, 1.0f);
    Vector2 hint_position = {
        static_cast<float>(get_display_width()) / 2 - hint_size.x / 2 + hint_offset.x,
        static_cast<float>(get_display_height()) / 2 - hint_size.y / 2 + hint_offset.y
    };

    DrawTextEx(menu_font, title_text.c_str(), title_position, title_font_size, 1.0f, WHITE);
    DrawTextEx(menu_font, hint_text.c_str(), hint_position, hint_font_size, 1.0f, GRAY);
}

