#include <graphics/scene.h>

#include "raylib.h"

#include "globals.h"
#include "player.h"
#include "graphics_old.h"
#include "assets.h"
#include "utilities.h"
#include "graphics/scenes/menu.h"

void update_game() {
    game_frame++;

    // TODO

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        move_player_horizontally(MOVEMENT_SPEED);
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        move_player_horizontally(-MOVEMENT_SPEED);
    }

    // Calculating collisions to decide whether the player is allowed to jump: don't want them to suction cup to the ceiling or jump midair
    is_player_on_ground = is_colliding({player_pos.x, player_pos.y + 0.1f}, WALL);
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && is_player_on_ground) {
        player_y_velocity = -JUMP_STRENGTH;
    }

    update_player();
}

void draw_game() {
    // TODO

    ClearBackground(BLACK);
    draw_current_scene();
}

int main() {
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);

    load_fonts();
    load_images();
    load_sounds();

    load_level(LevelPosition { 0, 0, 0 });

    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }

    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
