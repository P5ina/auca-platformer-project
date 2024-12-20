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
    shape_def.friction = 0.0f;
    shape_def.filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    shape_def.filter.maskBits = PhysicsLayers::WALL_LAYER | PhysicsLayers::GHOST_LAYER;
    const float height = 0.9f;
    const float radius = 0.3f;
    b2Capsule collider = {
        .center1 = {0, 0.5f - radius},
        .center2 = {0, 0.5f - height + radius },
        .radius = radius
    };
    b2CreateCapsuleShape(player_body_id, &shape_def, &collider);

    game_state->player = std::make_unique<Player>(Player {
        .body_id = player_body_id,
        // Quick fix for jump animation
        .jump_timer = 100.0f,
    });
}

void despawn_player(std::unique_ptr<GameState> &game_state) {
    if (game_state->player == nullptr)
        return;

    b2DestroyBody(game_state->player->body_id);
    game_state->player = nullptr;
}

void update_player(GameState *game_state, GameInput *game_input, float delta) {
    const float movement_speed = 5.0f;

    if (game_state->player == nullptr)
        return;

    if (game_state->player->movement_locked)
        return;

    if (game_input->jump) {
        jump_player(game_state);
    }

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    velocity.x = game_input->horizontal_movement * movement_speed;

    if (velocity.x > 0) {
        game_state->player->is_facing_left = false;
    }
    else if (velocity.x < 0) {
        game_state->player->is_facing_left = true;
    }

    game_state->player->jump_timer += delta;

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);
}

void jump_player(GameState *game_state) {
    constexpr float JUMP_FORCE = -8.0f;
    constexpr float JUMP_COOLDOWN = 0.3f;

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    bool g = is_grounded(game_state->loaded_level->world_id, game_state->player);
    if (g && game_state->player->jump_timer >= JUMP_COOLDOWN) {
        game_state->player->jump_timer = 0;
        velocity.y = JUMP_FORCE;
        PlaySound(jump_sound);
    }

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);
}

struct GroundCastContext {
    bool is_hit;
};

float ground_cast_callback(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void* context) {
    auto* ground_cast_context = static_cast<GroundCastContext *>(context);
    ground_cast_context->is_hit = true;
    return fraction;
}

bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player) {
    GroundCastContext context{};
    auto *shapes = new b2ShapeId[1];
    b2Body_GetShapes(player->body_id, shapes, 1);
    b2Capsule player_capsule = b2Shape_GetCapsule(shapes[0]);

    b2Capsule cast_capsule = b2Capsule {
        .center1 = player_capsule.center1,
        .center2 = player_capsule.center2,
        .radius = player_capsule.radius - 0.05f
    };

    b2Transform origin_transform;
    origin_transform.p = b2Body_GetPosition(player->body_id);
    origin_transform.q = b2Rot_identity;

    b2Vec2 direction = b2Vec2{0.0f, 0.2f};

    b2QueryFilter filter;
    filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    filter.maskBits = PhysicsLayers::WALL_LAYER;

    b2World_CastCapsule(world_id, &cast_capsule, origin_transform, direction, filter, ground_cast_callback, &context);

    return context.is_hit;
}
