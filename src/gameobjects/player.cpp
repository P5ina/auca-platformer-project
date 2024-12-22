//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "player.h"

#include <globals.h>
#include <iostream>
#include <graphics/metrics.h>
#include <sys/stat.h>

#include "gameobject_user_data.h"
#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state, Vector2 position, b2Vec2 initial_velocity) {
    b2BodyDef player_body_def = b2DefaultBodyDef();
    player_body_def.type = b2_dynamicBody;
    player_body_def.position = {position.x, position.y - 0.5f};
    player_body_def.fixedRotation = true;
    player_body_def.linearVelocity = initial_velocity;
    b2BodyId player_body_id = b2CreateBody(game_state->loaded_level->world_id, &player_body_def);

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 10.0f; // kg/m^2
    shape_def.friction = 0.0f;
    shape_def.filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    shape_def.filter.maskBits = PhysicsLayers::WALL_LAYER | PhysicsLayers::GHOST_LAYER |
                                PhysicsLayers::COLLECTABLE_LAYER;
    shape_def.userData = new GameObjectUserData{
        .type = GAME_OBJECT_PLAYER
    };

    const float height = 0.9f;
    const float radius = 0.3f;
    b2Capsule collider = {
        .center1 = {0, 0.5f - radius},
        .center2 = {0, 0.5f - height + radius},
        .radius = radius
    };
    b2CreateCapsuleShape(player_body_id, &shape_def, &collider);

    game_state->player = std::make_unique<Player>(Player{
            .body_id = player_body_id,
            // Quick fix for jump animation
            .jump_timer = 100.0f,
            .magnet_cursor = {
                .position = {
                    static_cast<float>(get_display_width()) / 2,
                    static_cast<float>(get_display_height()) / 2,
                }
            }
        }
    );
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
    } else if (velocity.x < 0) {
        game_state->player->is_facing_left = true;
    }

    game_state->player->jump_timer += delta;

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);
}

void clamp_player_position(const Level *level, const Player *player) {
    b2Vec2 pos = b2Body_GetPosition(player->body_id);
    auto width = static_cast<float>(level->columns);

    b2Vec2 velocity = b2Body_GetLinearVelocity(player->body_id);

    if (velocity.x > 0 && pos.x > width - 0.1f) {
        velocity.x = width - 0.1f - pos.x;
    }
    if (velocity.x < 0 && pos.x < 0.1f) {
        velocity.x = 0.1f - pos.x;
    }

    b2Body_SetLinearVelocity(player->body_id, velocity);
}


void jump_player(GameState *game_state) {
    constexpr float JUMP_FORCE = -8.0f;
    constexpr float JUMP_COOLDOWN = 0.3f;

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    bool g = is_grounded(game_state->loaded_level->world_id, game_state->player);
    if (g && game_state->player->jump_timer >= JUMP_COOLDOWN) {
        game_state->player->jump_timer = 0;
        velocity.y = JUMP_FORCE;
        PlaySound(*game_state->assets->sounds->jump_sound);
    }

    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);
}

struct GroundCastContext {
    bool is_hit;
};

float ground_cast_callback(b2ShapeId shape_id, b2Vec2 point, b2Vec2 normal, float fraction, void *context) {
    auto *ground_cast_context = static_cast<GroundCastContext *>(context);
    ground_cast_context->is_hit = true;
    return fraction;
}

bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player) {
    GroundCastContext context{};
    auto *shapes = new b2ShapeId[1];
    b2Body_GetShapes(player->body_id, shapes, 1);
    b2Capsule player_capsule = b2Shape_GetCapsule(shapes[0]);

    b2Capsule cast_capsule = b2Capsule{
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

Vector2i get_next_level_offset(const Level* level, const Player* player) {
    b2Vec2 pos = b2Body_GetPosition(player->body_id);

    if (pos.x < 0.1f) {
        return Vector2i { -1, 0 };
    }
    if (pos.y < 0) {
        return Vector2i { 0, -1 };
    }
    if (pos.x > static_cast<float>(level->columns) - 0.1f) {
        return Vector2i { 1, 0 };
    }
    if (pos.y > static_cast<float>(level->rows)) {
        return Vector2i { 0, 1 };
    }

    return Vector2i { 0, 0 };
}

Vector2 get_next_level_player_position(const Level* level, const Player* player) {
    b2Vec2 pos = b2Body_GetPosition(player->body_id);
    auto width = static_cast<float>(level->columns);
    auto height = static_cast<float>(level->rows);

    pos.y += 0.5f;

    if (pos.x < 0.1f) {
        return { width - 0.1f, pos.y };
    }
    if (pos.y - 0.5f < 0) {
        return { pos.x, height };
    }
    if (pos.x > width - 0.1f) {
        return { 0.1f, pos.y };
    }
    if (pos.y - 0.5f > height) {
        return { pos.x, 0 };
    }
    return { pos.x, pos.y };
}
