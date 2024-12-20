//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "player.h"

#include <globals.h>
#include <iostream>
#include <sys/stat.h>

#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state, Vector2 position) {
    b2BodyDef player_body_def = b2DefaultBodyDef();
    player_body_def.type = b2_dynamicBody;
    player_body_def.position = {position.x + 0.5f, position.y - 0.5f};
    player_body_def.fixedRotation = true;
    b2BodyId player_body_id = b2CreateBody(game_state->loaded_level->world_id, &player_body_def);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 10.0f; // kg/m^2
    const float height = 0.9f;
    const float radius = 0.3f;
    b2Capsule collider = {
        .center1 = {0, 0.5f - radius},
        .center2 = {0, 0.5f - height + radius },
        .radius = radius
    };
    b2CreateCapsuleShape(player_body_id, &shape_def, &collider);

    game_state->player = std::make_unique<Player>(Player{
        .body_id = player_body_id
    });
}

void despawn_player(std::unique_ptr<GameState> &game_state) {
    if (game_state->player == nullptr)
        return;

    b2DestroyBody(game_state->player->body_id);
    game_state->player = nullptr;
}

void update_player(GameState *game_state, GameInput *game_input, float delta) {
    if (game_state->player == nullptr)
        return;

    const float movement_speed = 5.0f;
    const float jump_cooldown = 0.2f;

    if (game_input->jump && game_state->player->jump_timer >= jump_cooldown) {
        jump_player(game_state);
    }

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    velocity.x = game_input->horizontal_movement * movement_speed;

    game_state->player->jump_timer += delta;

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

void jump_player(GameState *game_state) {
    const float jump_force = -8.0f;

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    bool g = is_grounded(game_state->loaded_level->world_id, game_state->player);
    if (g) {
        game_state->player->jump_timer = 0;
        velocity.y = jump_force;
    }

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);
}

bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player) {
    b2Vec2 origin = b2Body_GetPosition(player->body_id);
    b2Vec2 direction = b2Vec2{0.0f, 0.6f};
    b2QueryFilter filter = b2DefaultQueryFilter();
    filter.maskBits = static_cast<uint32_t>(PhysicsCategories::WALL);
    b2RayResult result = b2World_CastRayClosest(world_id, origin, direction, filter);

    return result.hit;
}
