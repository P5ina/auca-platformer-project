//
// Created by Timur Turatbekov on 22.12.2024.
//

#include "pause.h"

#include <globals.h>
#include <graphics/metrics.h>
#include <sys/stat.h>

#include "raylib.h"
#include "raymath.h"

void update_pause_menu(PauseMenuState *state, GameInput *game_input) {
    if (game_input->pause_pressed) {
        state->is_paused = !state->is_paused;
        if (state->is_paused) {
            EnableCursor();
            state->selected_option = 0;
            state->next_scene = std::nullopt;
        } else {
            DisableCursor();
        }
    }

    if (!state->is_paused) {
        return;
    }

    if (game_input->select_up_pressed) {
        state->selected_option = state->selected_option + 2 - 1;
    }
    if (game_input->select_down_pressed) {
        state->selected_option = state->selected_option + 1;
    }
    state->selected_option %= 2;

    if (game_input->select_pressed) {
        switch (state->selected_option) {
            case PAUSE_OPTION_CONTINUE: {
                state->is_paused = false;
                DisableCursor();
                break;
            }
            case PAUSE_OPTION_EXIT: {
                state->is_paused = false;
                state->next_scene = Scene::MENU_SCENE;
            }
        }
    }
}

void draw_pause_menu(PauseMenuState *state, Assets *assets) {
    if (!state->is_paused) {
        return;
    }

    DrawRectangle(0, 0, get_display_width(), get_display_height(), Fade(BLACK, 0.6f));

    float ui_scale = get_ui_scale();
    float title_font_size = 32.0f * ui_scale;
    Vector2 title_offset = Vector2Scale({ 0.0f, 40.0f }, ui_scale);
    std::string title_text = "PAUSE";
    Vector2 title_size = MeasureTextEx(menu_font, title_text.c_str(), title_font_size, 1.0f);
    Vector2 title_position = {
        static_cast<float>(get_display_width()) / 2 - title_size.x / 2 + title_offset.x,
        title_offset.y
    };

    float hint_font_size = 10.0f * ui_scale;
    Vector2 hint_offset = Vector2Scale({ 0.0f, -20.0f }, ui_scale);
    std::string hint_text = "W and S to move option   Space to confirm";
    Vector2 hint_size = MeasureTextEx(menu_font, hint_text.c_str(), hint_font_size, 1.0f);
    Vector2 hint_position = {
        static_cast<float>(get_display_width()) / 2 - hint_size.x / 2 + hint_offset.x,
        static_cast<float>(get_display_height()) - hint_size.y + hint_offset.y
    };

    DrawTextEx(menu_font, title_text.c_str(), title_position, title_font_size, 1.0f, WHITE);
    DrawTextEx(menu_font, hint_text.c_str(), hint_position, hint_font_size, 1.0f, GRAY);

    draw_pause_menu_option(PAUSE_OPTION_CONTINUE, state, assets);
    draw_pause_menu_option(PAUSE_OPTION_EXIT, state, assets);
}

void draw_pause_menu_option(PauseMenuOption option, PauseMenuState *state, Assets *assets) {
    std::string text;
    float option_offset_y = 0;
    switch (option) {
        case PAUSE_OPTION_CONTINUE: {
            text = "Continue";
            option_offset_y = 0;
            break;
        }
        case PAUSE_OPTION_EXIT: {
            text = "Exit";
            option_offset_y = 30.0f;
            break;
        }
        default: {
            text = "Unknown";
            option_offset_y = 60.0f;
            break;
        }
    }

    float ui_scale = get_ui_scale();
    float font_size = 18.0f * ui_scale;
    option_offset_y *= ui_scale;
    Vector2 text_offset = Vector2Scale({ 0.0f, 10.0f }, ui_scale);
    Vector2 text_size = MeasureTextEx(menu_font, text.c_str(), font_size, 1.0f);
    Vector2 text_position = {
        static_cast<float>(get_display_width()) / 2 - text_size.x / 2 + text_offset.x,
        static_cast<float>(get_display_height()) / 2 + text_offset.y + option_offset_y,
    };

    DrawTextEx(menu_font, text.c_str(), text_position, font_size, 1.0f, WHITE);

    if (state->selected_option == option) {
        Rectangle pointer_dest = {
            text_position.x - font_size - 4.0f * ui_scale,
            text_position.y,
            font_size,
            font_size
        };
        Rectangle pointer_source = {
            0, 0,
            static_cast<float>(assets->images->pointer_texture.width),
            static_cast<float>(assets->images->pointer_texture.height)
        };

        DrawTexturePro(assets->images->pointer_texture, pointer_source, pointer_dest, { 0, 0 }, 0, WHITE);
    }
}


