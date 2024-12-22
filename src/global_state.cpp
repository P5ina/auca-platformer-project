//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "global_state.h"

#include <globals.h>
#include <iostream>
#include <gameobjects/magnet.h>
#include <gameobjects/player.h>
#include <graphics/metrics.h>
#include <graphics/scenes/menu.h>

void update_game(std::unique_ptr<GameState> &game_state, float delta) {
    game_frame++;

    const auto inputs = std::make_unique<GameInput>(read_game_input());

    if (inputs->toggle_debug_mode) {
        game_state->debug_mode = !game_state->debug_mode;
    }

    int monitor = GetCurrentMonitor();
    int monitor_width = GetMonitorWidth(monitor);
    int monitor_height = GetMonitorHeight(monitor);

    if (inputs->toggle_full_screen) {
        if (IsWindowFullscreen()) {
            ToggleFullscreen();
            SetWindowSize(INITIAL_WIDTH, INITIAL_HEIGHT);
        } else {
            SetWindowSize(monitor_width, monitor_height);
            ToggleFullscreen();
        }
    }

    if (IsWindowFullscreen() &&
        (monitor_width != GetScreenWidth() || monitor_height != GetScreenHeight())) {
        SetWindowSize(monitor_width, monitor_height);
    }

    if (game_state->scene == Scene::MENU_SCENE) {
        update_menu(game_state.get(), inputs.get());
    }

    if (game_state->scene == Scene::LEVEL_SCENE) {
        update_pause_menu(&game_state->pause_menu_state, inputs.get());
        if (game_state->pause_menu_state.next_scene != std::nullopt) {
            despawn_player(game_state);
            unload_level(std::move(game_state->loaded_level));

            game_state->loaded_level = load_level({ 0, 0 });
            game_state->loaded_dialogue = nullptr;
            spawn_player(game_state, PLAYER_SPAWN, { 0, 0 });
            game_state->scene = Scene::MENU_SCENE;
            init_main_menu(game_state.get());
            game_state->pause_menu_state.next_scene = std::nullopt;

            EnableCursor();
        }
    }

    if (!game_state->pause_menu_state.is_paused && game_state->scene == Scene::LEVEL_SCENE) {
        update_player(game_state.get(), inputs.get(), delta);
        update_dialogue(game_state->loaded_dialogue.get(), inputs.get(), game_state->assets.get());
        Vector2i level_offset = get_next_level_offset(game_state->loaded_level.get(), game_state->player.get());
        if (level_offset.x != 0 || level_offset.y != 0) {
            Vector2i next_level = vector2i_add(game_state->loaded_level->position, level_offset);
            if (has_level(next_level)) {
                Vector2 next_position = get_next_level_player_position(game_state->loaded_level.get(), game_state->player.get());
                b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
                despawn_player(game_state);
                unload_level(std::move(game_state->loaded_level));
                game_state->loaded_level = load_level(next_level);
                spawn_player(game_state, next_position, velocity);
            }
        }
        clamp_player_position(game_state->loaded_level.get(), game_state->player.get());

        if (is_dialogue_ended(game_state->loaded_dialogue.get()) && game_state->loaded_dialogue != nullptr) {
            game_state->loaded_dialogue = nullptr;
        }

        update_magnet_cursor(&game_state->player->magnet_cursor, game_state->loaded_level.get(), inputs.get());
    }
}
