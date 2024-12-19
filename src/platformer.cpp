#include <global_state.h>
#include <input.h>
#include <characters/player.h>
#include <graphics/scene.h>
#include <old_assets.h>

#include "raylib.h"

#include "globals.h"

void update_game(std::unique_ptr<GameState> &game_state, float delta) {
    game_frame++;

    std::unique_ptr<GameInput> inputs = std::make_unique<GameInput>(read_game_input());

    update_player(game_state.get(), inputs.get(), delta);
}

void draw_game(std::unique_ptr<GameState> &game_state) {
    // TODO

    ClearBackground(BLACK);
    draw_current_scene(game_state);
}

int main() {
    const float PHYSICS_DELTA_TIME = 1.0f / 60.0f;
    const int PHYSICS_SUB_STEP = 4;

    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();

    auto game_state = std::make_unique<GameState>(
        Scene::LEVEL_SCENE,
        nullptr
    );

    load_level(game_state, LevelPosition { 0, 0, 0 });

    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game(game_state, PHYSICS_DELTA_TIME);
        if (game_state != nullptr && game_state->loaded_level != nullptr) {
            b2World_Step(game_state->loaded_level->world_id, PHYSICS_DELTA_TIME, PHYSICS_SUB_STEP);
        }
        draw_game(game_state);

        EndDrawing();
    }

    unload_level(game_state);
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
