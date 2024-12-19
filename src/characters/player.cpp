//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "player.h"

#include <globals.h>
#include <iostream>
#include <sys/stat.h>

#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state) {
    const std::unique_ptr<Level> &level = game_state->loaded_level;

    for (size_t row = 0; row < level->rows; ++row) {
        for (size_t column = 0; column < level->columns; ++column) {
            LevelTile tile = level->tiles[row * level->columns + column];
            if (tile.type == LevelTileType::PLAYER_SPAWN) {
                b2BodyDef player_body_def = b2DefaultBodyDef();
                player_body_def.type = b2_dynamicBody;
                player_body_def.position = {static_cast<float>(column) + 0.5f, static_cast<float>(row) - 0.5f};
                player_body_def.fixedRotation = true;
                b2BodyId player_body_id = b2CreateBody(game_state->loaded_level->world_id, &player_body_def);

                b2ShapeDef shape_def = b2DefaultShapeDef();
                shape_def.density = 10.0f; // kg/m^2
                //shape_def.friction = 0.0f;
                const float height = 0.9f;
                const float radius = 0.3f;
                b2Capsule collider = {
                    .center1 = {0, 0.5f - radius},
                    .center2 = {0, 0.5f - height + radius },
                    .radius = radius
                };
                // b2Circle collider = {
                //     .center = {0, 0},
                //     .radius = 0.5f
                // };
                b2CreateCapsuleShape(player_body_id, &shape_def, &collider);

                game_state->player = std::make_unique<Player>(Player{
                    .body_id = player_body_id
                });
                return;
            }
        }
    }
}

void despawn_player(std::unique_ptr<GameState> &game_state) {
    // TODO: Deallocate player
}


void move_player_horizontally(std::unique_ptr<GameState> &game_state, float delta) {
    // float next_x = player_pos.x + delta;
    // if (!is_colliding(game_state->loaded_level, {next_x, player_pos.y}, WALL)) {
    //     player_pos.x = next_x;
    // } else {
    //     player_pos.x = roundf(player_pos.x);
    // }
}

void update_player(GameState *game_state, GameInput *game_input, float delta) {
    const float movement_speed = 5.0f;
    const float jump_force = -12.0f;
    const float jump_cooldown = 0.2f;

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    velocity.x = game_input->horizontal_movement * movement_speed;

    game_state->player->jump_timer += delta;

    bool g = is_grounded(game_state->loaded_level->world_id, game_state->player);
    if (g && game_input->jump && game_state->player->jump_timer >= jump_cooldown) {
        game_state->player->jump_timer = 0;
        velocity.y = jump_force;
    }

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);

    // if (is_colliding(game_state->loaded_level, player_pos, COIN)) {
    //     int index = get_collider_tile_index(game_state->loaded_level, player_pos, COIN);
    //     set_tile_at_index(game_state->loaded_level, index, AIR);
    //     player_score += 10;
    //     PlaySound(coin_sound);
    // }
    // if (is_colliding(game_state->loaded_level, player_pos, EXIT)) {
    //     // TODO
    // }
}

bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player) {
    b2Vec2 origin = b2Body_GetPosition(player->body_id);
    b2Vec2 direction = b2Vec2{0.0f, 0.6f};
    b2QueryFilter filter = b2DefaultQueryFilter();
    filter.maskBits = static_cast<uint32_t>(PhysicsCategories::WALL);
    b2RayResult result = b2World_CastRayClosest(world_id, origin, direction, filter);

    return result.hit;
}
