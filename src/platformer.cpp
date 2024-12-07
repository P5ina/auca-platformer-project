#include <global_state.h>
#include <graphics/scene.h>

#include "raylib.h"

#include "globals.h"
#include "player.h"
#include "graphics_old.h"
#include "old_assets.h"
#include "utilities.h"
#include "graphics/scenes/menu.h"

void update_game(std::unique_ptr<GameState> &game_state) {
    game_frame++;

    // TODO

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        move_player_horizontally(game_state, MOVEMENT_SPEED);
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        move_player_horizontally(game_state, -MOVEMENT_SPEED);
    }

    // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
    is_player_on_ground = is_colliding(game_state->loaded_level, {player_pos.x, player_pos.y + 0.1f}, WALL);
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
        player_y_velocity = -JUMP_STRENGTH;
    }

    update_player(game_state);
}

void draw_game(std::unique_ptr<GameState> &game_state) {
    // TODO

    ClearBackground(BLACK);
    draw_current_scene(game_state);
}

int main() {
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

        update_game(game_state);
        draw_game(game_state);

        EndDrawing();
    }

    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
