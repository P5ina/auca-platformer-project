#include <global_state.h>
#include <input.h>
#include <iostream>
#include <gameobjects/player.h>
#include <graphics/scene.h>
#include <old_assets.h>
#include <gameobjects/gameobject_user_data.h>
#include <graphics/metrics.h>
#include <graphics/scenes/level_scene.h>
#include <graphics/scenes/menu.h>

#include "raylib.h"

#include "globals.h"

void draw_game(std::unique_ptr<GameState> &game_state, RenderTexture2D &target) {
    if (game_state->last_resolution.x != GetScreenWidth() ||
        game_state->last_resolution.y != GetScreenHeight()) {
        UnloadRenderTexture(target);
        target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
        game_state->last_resolution = {GetScreenWidth(), GetScreenHeight()};
    }

    BeginTextureMode(target);
    ClearBackground(BLACK);
    draw_current_scene(game_state);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    if (!game_state->debug_mode) {
        BeginShaderMode(crt_shader);
    }

    DrawTextureRec(target.texture,
                   Rectangle{
                       0, 0,
                       static_cast<float>(target.texture.width),
                       static_cast<float>(-target.texture.height)
                   },
                   Vector2{0, 0},
                   WHITE
    );

    if (!game_state->debug_mode) {
        EndShaderMode();
    }
    EndDrawing();
}

void process_events(GameState* game_state) {
    b2SensorEvents sensorEvents = b2World_GetSensorEvents(game_state->loaded_level->world_id);
    for (int i = 0; i < sensorEvents.beginCount; ++i) {
        std::cout << "Touch event appear" << std::endl;

        b2SensorBeginTouchEvent *beginTouch = sensorEvents.beginEvents + i;
        auto *visitor_user_data = static_cast<GameObjectUserData *>(b2Shape_GetUserData(beginTouch->visitorShapeId));
        auto *sensor_user_data = static_cast<GameObjectUserData *>(b2Shape_GetUserData(beginTouch->sensorShapeId));

        if (visitor_user_data == nullptr || sensor_user_data == nullptr) {
            continue;
        }

        if (visitor_user_data->type == GAME_OBJECT_PLAYER &&
            sensor_user_data->type == GAME_OBJECT_KNOWLEDGE_BOOK) {
            std::cout << "Book touched" << std::endl;

            for (auto &book : game_state->loaded_level->books) {
                if (book.id == sensor_user_data->knowledge_book_id) {
                    collect_knowledge_book(&book);
                    game_state->loaded_dialogue = std::make_unique<Dialogue>(Dialogue {
                        .lines = book.dialogue_lines
                    });
                }
            }
        }

        // process begin event
    }
}

int main() {
    const float PHYSICS_DELTA_TIME = 1.0f / 60.0f;
    const int PHYSICS_SUB_STEP = 4;

    InitWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Platformer");
    SetTargetFPS(60);

    SetExitKey(KEY_NULL);

    auto game_state = std::make_unique<GameState>(GameState{
            .scene = Scene::MENU_SCENE,
            .assets = std::make_unique<Assets>(),
        }
    );

    load_fonts();
    game_state->assets->images = load_images();
    load_shaders();
    game_state->assets->sounds = load_sounds();
    game_state->loaded_level = load_level(PLAYER_LEVEL_SPAWN);
    spawn_player(game_state, PLAYER_SPAWN, { 0, 0 });
    init_main_menu(game_state.get());

    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    while (!(WindowShouldClose() || game_state->close)) {

        if (!game_state->pause_menu_state.is_paused) {
            if (game_state != nullptr && game_state->loaded_level != nullptr) {
                b2World_Step(game_state->loaded_level->world_id, PHYSICS_DELTA_TIME, PHYSICS_SUB_STEP);
            }
            process_events(game_state.get());
        }
        update_game(game_state, PHYSICS_DELTA_TIME);
        draw_game(game_state, target);
    }

    UnloadRenderTexture(target);
    unload_level(std::move(game_state->loaded_level));
    unload_sounds(std::move(game_state->assets->sounds));
    unload_images(std::move(game_state->assets->images));
    unload_shaders();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
