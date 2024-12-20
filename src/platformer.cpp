#include <global_state.h>
#include <input.h>
#include <gameobjects/player.h>
#include <graphics/scene.h>
#include <old_assets.h>
#include <graphics/scenes/menu.h>

#include "raylib.h"

#include "globals.h"

void update_game(std::unique_ptr<GameState> &game_state, float delta) {
    game_frame++;

    const auto inputs = std::make_unique<GameInput>(read_game_input());

    if (inputs->toggle_debug_mode) {
        game_state->debug_mode = !game_state->debug_mode;
    }

    if (inputs->toggle_full_screen) {
        int width = GetMonitorWidth(0);
        int height = GetMonitorHeight(0);
        ToggleFullscreen();
        if (IsWindowFullscreen()) {
            SetWindowSize(width, height);
        } else {
            SetWindowSize(1280, 720);
        }
        derive_graphics_metrics_from_level(game_state->loaded_level);
    }

    update_player(game_state.get(), inputs.get(), delta);

    if (game_state->scene == Scene::MENU_SCENE) {
        update_menu(game_state.get(), inputs.get());
    }
}

void draw_game(std::unique_ptr<GameState> &game_state, RenderTexture2D &target) {
    if (game_state->last_resolution.x != GetScreenWidth() ||
        game_state->last_resolution.y != GetScreenHeight()) {
        UnloadRenderTexture(target);
        target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        game_state->last_resolution = { GetScreenWidth(), GetScreenHeight() };
    }

    BeginTextureMode(target);
        ClearBackground(BLACK);
        draw_current_scene(game_state);
    EndTextureMode();

    BeginDrawing();
        ClearBackground(BLACK);
        BeginShaderMode(crt_shader);
            DrawTextureRec(target.texture,
                Rectangle {
                    0, 0,
                    static_cast<float>(target.texture.width),
                    static_cast<float>(-target.texture.height)
                },
                Vector2 { 0, 0 },
                WHITE);

        EndShaderMode();
    EndDrawing();
}

int main() {
    const float PHYSICS_DELTA_TIME = 1.0f / 60.0f;
    const int PHYSICS_SUB_STEP = 4;

    InitWindow(1280, 720, "Platformer");
    SetTargetFPS(60);

    auto game_state = std::make_unique<GameState>(GameState {
        .scene = Scene::MENU_SCENE,
        .loaded_level = nullptr,
        .assets = std::make_unique<Assets>(),
    });

    load_fonts();
    game_state->assets->images = load_images();
    load_shaders();
    load_sounds();
    load_level(game_state, LevelPosition { 0, 0, 0 });
    spawn_player(game_state, { 7.5, 8 });
    init_main_menu(game_state.get());

    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    while (!WindowShouldClose()) {
        update_game(game_state, PHYSICS_DELTA_TIME);
        if (game_state != nullptr && game_state->loaded_level != nullptr) {
            b2World_Step(game_state->loaded_level->world_id, PHYSICS_DELTA_TIME, PHYSICS_SUB_STEP);
        }
        draw_game(game_state, target);
    }

    UnloadRenderTexture(target);
    unload_level(game_state);
    unload_sounds();
    unload_images(std::move(game_state->assets->images)) ;
    unload_shaders();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
