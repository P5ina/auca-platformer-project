//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "menu.h"

#include <algorithm>
#include <globals.h>
#include <input.h>
#include <random>
#include <graphics/animation.h>
#include <string>
#include <gameobjects/player.h>
#include <graphics/metrics.h>

#include "raylib.h"
#include "raymath.h"

const std::string title_text = "Deep Woods";

void draw_menu(GameState *game_state) {
    game_state->main_menu_state.elapsed_time += GetFrameTime();
    draw_game_title(game_state);
    draw_start_message(game_state);
}

void draw_game_title(GameState *game_state) {
    const float FONT_SIZE = 64.0f * get_ui_scale();
    const float TEXT_SPACING_START = 15.0f;
    const float TEXT_SPACING_END = 4.0f;
    const Vector2 TEXT_OFFSET = Vector2Scale({0, 60.0f}, get_ui_scale());
    const Color TEXT_COLOR_START = ColorAlpha(get_color_from_hex("2700B3"), 0.0f);
    const Color TEXT_COLOR_END = get_color_from_hex("8000FF");

    const float ANIMATION_DURATION = 4.5f;
    const float ANIMATION_DELAY = 1.0f;
    const float START_OFFSET_Y = 100.0f * get_ui_scale();
    const float WOBBLE_FREQUENCY = 3.0f;
    const float WOBBLE_AMPLITUDE = 80.0f * get_ui_scale();
    const float CHAR_ANIMATION_OVERLAP = 0.95f;

    float sumTextWidthStart = 0.0f;
    float sumTextWidthEnd = 0.0f;
    for (int i = 0; i < TextLength(title_text.c_str()); i++) {
        const char *element = TextSubtext(title_text.c_str(), i, 1);
        float elementWidth = MeasureTextEx(menu_font, element, FONT_SIZE, 0).x;
        sumTextWidthStart += elementWidth + TEXT_SPACING_START;
        sumTextWidthEnd += elementWidth + TEXT_SPACING_END;
    }
    sumTextWidthStart -= TEXT_SPACING_START;
    sumTextWidthEnd -= TEXT_SPACING_END;

    float animation_progress = get_animation_value(game_state->main_menu_state.elapsed_time, ANIMATION_DURATION,
                                                   ANIMATION_DELAY
    );
    float current_offset_x_start = 0.0f;
    float current_offset_x_end = 0.0f;
    for (int i = 0; i < TextLength(title_text.c_str()); i++) {
        const char *element = TextSubtext(title_text.c_str(), i, 1);
        float elementWidth = MeasureTextEx(menu_font, element, FONT_SIZE, 0).x;
        int order = game_state->main_menu_state.title_char_appearing_order[i];
        float elementAnimationProgress = get_animation_value(
            animation_progress * (TextLength(title_text.c_str()) * (1.0f - CHAR_ANIMATION_OVERLAP) +
                                  CHAR_ANIMATION_OVERLAP), 1.0f, order * (1.0f - CHAR_ANIMATION_OVERLAP)
        );

        Vector2 startPosition = {
            static_cast<float>(get_display_width()) / 2 - (sumTextWidthStart / 2) + current_offset_x_start,
            0.0f + START_OFFSET_Y,
        };

        Vector2 endPosition = {
            static_cast<float>(get_display_width()) / 2 - (sumTextWidthEnd / 2) + current_offset_x_end,
            0.0f,
        };


        float wobbleX = sinf(elementAnimationProgress * WOBBLE_FREQUENCY) * WOBBLE_AMPLITUDE;
        if (order % 2 == 1) {
            wobbleX = -wobbleX;
        }
        Vector2 wobble = Vector2Scale({wobbleX, 0}, 1.0f - elementAnimationProgress);

        // Quad interpolation
        Vector2 position = Vector2Lerp(startPosition, endPosition, ease_out_sine(elementAnimationProgress));
        position = Vector2Add(position, TEXT_OFFSET);
        position = Vector2Add(position, wobble);

        Color color = ColorLerp(TEXT_COLOR_START, TEXT_COLOR_END, elementAnimationProgress);

        DrawTextEx(menu_font, element, position, FONT_SIZE, 0, color);
        current_offset_x_start += elementWidth + TEXT_SPACING_START;
        current_offset_x_end += elementWidth + TEXT_SPACING_END;
    }
}

void draw_start_message(GameState *game_state) {
    const float ANIMATION_DURATION = 1.5f;
    const float ANIMATION_DELAY = 5.0f;
    const float FONT_SIZE = 18.0f * get_ui_scale();
    const Vector2 TEXT_OFFSET = Vector2Scale({0, 30.0f}, get_ui_scale());

    const std::string text = "Press SPACE to Jump";

    Vector2 size = MeasureTextEx(menu_font, text.c_str(), FONT_SIZE, 1.0f);

    float animation_progress = get_animation_value(game_state->main_menu_state.elapsed_time, ANIMATION_DURATION,
                                                   ANIMATION_DELAY
    );
    auto position = Vector2{
        (static_cast<float>(get_display_width()) - size.x) / 2.0f + TEXT_OFFSET.x,
        (static_cast<float>(get_display_height()) - size.y) / 2.0f + TEXT_OFFSET.y
    };
    Color color = Fade(WHITE, animation_progress);
    DrawTextEx(menu_font, text.c_str(), position, FONT_SIZE, 1.0f, color);


    // const Vector2 RECT_SIZE = { 80.0f, 60.0f };
    //
    // Vector2 position = Vector2Add(
    //     Vector2{
    //         (static_cast<float>(GetScreenWidth()) - RECT_SIZE.x) / 2.0f,
    //         (static_cast<float>(GetScreenHeight()) - RECT_SIZE.y) / 2.0f
    //     },
    //     OFFSET);
    // DrawRectangleRec(
    //     Rectangle {
    //         position.x, position.y,
    //         RECT_SIZE.x, RECT_SIZE.y
    //     },
    //     WHITE);
}

void init_main_menu(GameState *game_state) {
    game_state->main_menu_state = {};
    game_state->player->movement_locked = true;

    // Generate random order for characters for appear
    unsigned int length = TextLength(title_text.c_str());
    game_state->main_menu_state.title_char_appearing_order = std::vector<int>(length);
    for (int i = 0; i < length; i++) {
        game_state->main_menu_state.title_char_appearing_order[i] = i;
    }

    auto rng = std::default_random_engine{};
    std::ranges::shuffle(game_state->main_menu_state.title_char_appearing_order, rng);

    PlayMusicStream(*game_state->assets->sounds->main_menu_music);
}

void update_menu(GameState *game_state, GameInput *game_input) {
    UpdateMusicStream(*game_state->assets->sounds->main_menu_music);

    if (game_state->main_menu_state.elapsed_time > 5.0f && game_input->jump) {
        StopMusicStream(*game_state->assets->sounds->main_menu_music);
        game_state->player->movement_locked = false;
        game_state->scene = Scene::LEVEL_SCENE;

        jump_player(game_state);

        game_state->loaded_dialogue = std::make_unique<Dialogue>(Dialogue {
            .lines = {
                "Welcome to Deep Woods. My name is DAMAGED FILE. This will be your first lesson. Use A, D and Space keys to move... Find books that I left for you, and you will know more about this world.",
                "This will be your first lesson. Use A, D and Space keys to move.",
                "Find books that I left for you, and you will know more about this world."
            }
        });
    }
}
