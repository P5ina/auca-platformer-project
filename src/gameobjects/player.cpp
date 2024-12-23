//
// Created by Timur Turatbekov on 09.12.2024.
//

#include "player.h"

#include <format>
#include <globals.h>
#include <iostream>
#include <graphics/metrics.h>
#include <sys/stat.h>

#include "gameobject_user_data.h"
#include "raymath.h"
#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state, Vector2 position, b2Vec2 initial_velocity) {
    b2BodyDef player_body_def = b2DefaultBodyDef();
    player_body_def.type = b2_dynamicBody;
    player_body_def.position = { position.x, position.y };
    player_body_def.fixedRotation = true;
    player_body_def.linearVelocity = initial_velocity;
    b2BodyId player_body_id = b2CreateBody(game_state->loaded_level->world_id, &player_body_def);
    add_player_shape(player_body_id, {0, 0});

    game_state->player = std::make_unique<Player>(Player{
            .body_id = player_body_id,
            .magnet_cursor = {
                .position = {
                    static_cast<float>(get_display_width()) / 2,
                    static_cast<float>(get_display_height()) / 2,
                }
            },
            // Quick fix for jump animation
            .jump_timer = 100.0f,
        }
    );
}

void add_player_shape(b2BodyId body_id, b2Vec2 offset) {
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 10.0f; // kg/m^2
    shape_def.friction = 0.0f;
    shape_def.filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    shape_def.filter.maskBits = PhysicsLayers::WALL_LAYER | PhysicsLayers::GHOST_LAYER |
                                PhysicsLayers::COLLECTABLE_LAYER | PhysicsLayers::MAGNET_BLOCK_LAYER;
    shape_def.userData = new GameObjectUserData{
        .type = GAME_OBJECT_PLAYER
    };

    constexpr float height = 0.9f;
    constexpr float radius = 0.3f;
    b2Capsule collider = {
        .center1 = {0 + offset.x, 0.5f - radius + offset.y},
        .center2 = {0 + offset.x, 0.5f - height + radius + offset.y},
        .radius = radius
    };
    b2CreateCapsuleShape(body_id, &shape_def, &collider);
}

void despawn_player(std::unique_ptr<GameState> &game_state) {
    if (game_state->player == nullptr)
        return;

    b2DestroyBody(game_state->player->body_id);
    game_state->player = nullptr;
}

void update_player(GameState *game_state, GameInput *game_input, float delta) {
    const float acceleration = 1.5f;
    const float movement_speed = 5.0f;
    const float ground_decay = 0.8f;
    const float air_decay = 0.95f;

    if (game_state->player == nullptr)
        return;

    game_state->player->is_grounded = is_grounded(game_state->loaded_level->world_id, game_state->player);
    game_state->player->jump_timer += delta;

    b2Vec2 velocity = b2Body_GetLinearVelocity(game_state->player->body_id);
    if (!game_state->player->movement_locked) {
        if (game_input->jump) {
            jump_player(game_state);
        }

        velocity = b2Body_GetLinearVelocity(game_state->player->body_id);

        float increment = game_input->horizontal_movement * acceleration;
        velocity.x = std::clamp(velocity.x + increment, -movement_speed, movement_speed);

        if (velocity.x > 0) {
            game_state->player->is_facing_left = false;
        } else if (velocity.x < 0) {
            game_state->player->is_facing_left = true;
        }
    }

    if (game_state->player->is_grounded && game_input->horizontal_movement == 0) {
        velocity.x *= ground_decay;
    }
    if (!game_state->player->is_grounded && game_input->horizontal_movement == 0) {
        velocity.x *= air_decay;
    }
    b2Body_SetLinearVelocity(game_state->player->body_id, velocity);

    update_magnet_player(game_state->player.get(), game_state->loaded_level.get());
    update_magnet_object(game_state->player.get(), game_state->loaded_level.get());
}

void update_magnet_player(Player *player, Level *level) {
    if (B2_IS_NULL(player->magnet_cursor.attached_to) || !player->magnet_cursor.is_player_moving) {
        return;
    }

    constexpr float ATTACHING_FORCE = 400.0f;
    constexpr float MAX_DIST = 5.0f;

    float force = 0.0f;
    if (player->magnet_cursor.is_pull) {
        force += ATTACHING_FORCE;
    }
    if (player->magnet_cursor.is_push) {
        force -= ATTACHING_FORCE;
    }

    b2Vec2 player_position = b2Body_GetPosition(player->body_id);
    b2Vec2 target_position = b2Body_GetPosition(player->magnet_cursor.attached_to);

    float distance = b2Distance(player_position, target_position);
    if (distance > MAX_DIST) {
        return;
    }

    b2Vec2 direction = b2Normalize(b2Sub(target_position, player_position));
    b2Vec2 add_velocity = b2MulSV(force / distance, direction);

    b2Body_ApplyForceToCenter(player->body_id, add_velocity, true);
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
    if (game_state->player->is_grounded && game_state->player->jump_timer >= JUMP_COOLDOWN) {
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
    filter.maskBits = PhysicsLayers::WALL_LAYER | PhysicsLayers::MAGNET_BLOCK_LAYER;

    b2World_CastCapsule(world_id, &cast_capsule, origin_transform, direction, filter, ground_cast_callback, &context);

    return context.is_hit;
}

Vector2i get_next_level_offset(const Level *level, const Player *player) {
    b2Vec2 pos = b2Body_GetPosition(player->body_id);

    if (pos.x < 0.1f) {
        return Vector2i{-1, 0};
    }
    if (pos.y < 0.1f) {
        return Vector2i{0, -1};
    }
    if (pos.x > static_cast<float>(level->columns) - 0.1f) {
        return Vector2i{1, 0};
    }
    if (pos.y > static_cast<float>(level->rows) - 0.1f) {
        return Vector2i{0, 1};
    }

    return Vector2i{0, 0};
}

Vector2 get_next_level_player_position(const Level *level, const Player *player) {
    b2Vec2 pos = b2Body_GetPosition(player->body_id);
    auto width = static_cast<float>(level->columns);
    auto height = static_cast<float>(level->rows);

    if (pos.x < 0.1f) {
        return {width - 0.1f, pos.y};
    }
    if (pos.y < 0.1f) {
        return {pos.x, height - 0.1f};
    }
    if (pos.x > width - 0.1f) {
        return {0.1f, pos.y};
    }
    if (pos.y > height - 0.1f) {
        return {pos.x, 0.1f};
    }
    return {pos.x, pos.y};
}

struct MagnetOverlapContext {
    b2BodyId body_id;
    bool hit;
};

bool magnet_overlap_callback(b2ShapeId shapeId, void *context) {
    auto magnet_context = static_cast<MagnetOverlapContext *>(context);
    b2BodyId body_id = b2Shape_GetBody(shapeId);
    magnet_context->body_id = body_id;
    magnet_context->hit = true;
    return false;
}

void update_magnet_cursor(Level *level, Player *player, GameInput *game_input) {
    MagnetCursor *magnet_cursor = &player->magnet_cursor;
    float ui_scale = get_ui_scale();
    float cursor_size = 30.0f * ui_scale;
    float tile_size = get_tile_screen_size(level);

    float width = get_display_width();
    float height = get_display_height();
    float mouse_sens = 1.0f;
    Vector2 mouse_delta = Vector2Scale(GetMouseDelta(), mouse_sens);

    magnet_cursor->delta = mouse_delta;

    if (B2_IS_NULL(magnet_cursor->attached_to)) {
        Vector2 cursor_pos = magnet_cursor->position;
        cursor_pos = Vector2Add(cursor_pos, mouse_delta);

        if (cursor_pos.x < cursor_size / 2) {
            cursor_pos = {cursor_size / 2, cursor_pos.y};
        }
        if (cursor_pos.x > width - cursor_size / 2) {
            cursor_pos = {width - cursor_size / 2, cursor_pos.y};
        }
        if (cursor_pos.y < cursor_size / 2) {
            cursor_pos = {cursor_pos.x, cursor_size / 2};
        }
        if (cursor_pos.y > height - cursor_size / 2) {
            cursor_pos = {cursor_pos.x, height - cursor_size / 2};
        }

        magnet_cursor->position = cursor_pos;
    } else {
        b2Vec2 target_position = b2Body_GetPosition(magnet_cursor->attached_to);
        Vector2 target_screen_position = get_tile_screen_position(level, {
                                                                      target_position.x, target_position.y
                                                                  });
        magnet_cursor->position = target_screen_position;
    }

    b2Vec2 cursor_level_position = {
        magnet_cursor->position.x / tile_size,
        magnet_cursor->position.y / tile_size
    };
    b2Circle query_circle = b2Circle{
        .center = {0, 0},
        .radius = (cursor_size / tile_size) / 2,
    };
    b2Transform query_transform;
    query_transform.p = cursor_level_position;
    query_transform.q = b2Rot_identity;
    b2QueryFilter query_filter = b2DefaultQueryFilter();
    query_filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    query_filter.maskBits = PhysicsLayers::MAGNET_BLOCK_LAYER;

    if (game_input->magnet_pull_pressed) {
        magnet_cursor->is_pull = true;
    }
    if (game_input->magnet_pull_released) {
        magnet_cursor->is_pull = false;
    }
    if (game_input->magnet_push_pressed) {
        magnet_cursor->is_push = true;
    }
    if (game_input->magnet_push_released) {
        magnet_cursor->is_push = false;
    }

    if ((magnet_cursor->is_pull || magnet_cursor->is_push) && B2_IS_NULL(magnet_cursor->attached_to)) {
        MagnetOverlapContext context{};
        b2World_OverlapCircle(level->world_id, &query_circle, query_transform, query_filter, magnet_overlap_callback,
                              &context);

        if (context.hit) {
            b2ShapeId shape_id;
            b2Body_GetShapes(context.body_id, &shape_id, 1);
            magnet_cursor->is_player_moving = true;

            switch (auto *user_data = static_cast<GameObjectUserData *>(b2Shape_GetUserData(shape_id)); user_data->
                type) {
                case GAME_OBJECT_MAGNET_BOX: {
                    if (magnet_cursor->is_pull) {
                        b2Vec2 target_position = b2Body_GetPosition(context.body_id);
                        float distance = b2Distance(
                            b2Body_GetPosition(player->body_id), target_position);

                        if (distance < MAX_MAGNET_DISTANCE) {
                            magnet_cursor->is_push = false;
                            magnet_cursor->is_player_moving = false;
                            magnet_cursor->target_pos = target_position;
                            magnet_cursor->attached_to = context.body_id;
                        }
                    }
                    break;
                }
                case GAME_OBJECT_MAGNET_BLOCK: {
                    magnet_cursor->is_player_moving = true;
                    magnet_cursor->attached_to = context.body_id;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }

    // Stop pulling
    if (!B2_IS_NULL(magnet_cursor->attached_to)) {
        if (!magnet_cursor->is_pull && !magnet_cursor->is_push) {
            magnet_cursor->attached_to = b2_nullBodyId;
            magnet_cursor->is_player_moving = false;
        }

        if (magnet_cursor->is_push && !magnet_cursor->is_player_moving) {
            magnet_cursor->is_pull = false;

            const float shoot_force = 300.0f;

            b2Vec2 player_position = b2Body_GetPosition(player->body_id);
            b2Vec2 current_position = b2Body_GetPosition(magnet_cursor->attached_to);
            b2Vec2 impulse = b2Normalize(b2Sub(current_position, player_position));

            b2Body_ApplyLinearImpulseToCenter(magnet_cursor->attached_to, b2MulSV(shoot_force, impulse), true);

            magnet_cursor->attached_to = b2_nullBodyId;
            magnet_cursor->is_player_moving = false;
        }
    }
}

void update_magnet_object(Player *player, Level *level) {
    MagnetCursor *magnet_cursor = &player->magnet_cursor;
    if (B2_IS_NULL(magnet_cursor->attached_to) || magnet_cursor->is_player_moving) {
        return;
    }

    b2Vec2 player_position = b2Body_GetPosition(player->body_id);
    b2Vec2 delta = {
        magnet_cursor->delta.x / get_tile_screen_size(level),
        magnet_cursor->delta.y / get_tile_screen_size(level)
    };
    b2Vec2 new_position = b2Add(magnet_cursor->target_pos, delta);
    b2Vec2 to_target_position = b2Sub(new_position, player_position);
    float length = b2Length(to_target_position);
    if (length > MAX_MAGNET_DISTANCE) {
        new_position = b2Add(player_position, b2MulSV(MAX_MAGNET_DISTANCE, b2Normalize(to_target_position)));
    }

    std::cout << std::format("Update magnet object delta {}, {}", delta.x, delta.y) << std::endl;
    std::cout << std::format("Update magnet object {}, {}", new_position.x, new_position.y) << std::endl;

    b2Vec2 current_position = b2Body_GetPosition(magnet_cursor->attached_to);
    const b2Vec2 force = b2Sub(new_position, current_position);
    b2Body_SetLinearVelocity(magnet_cursor->attached_to, force);

    magnet_cursor->target_pos = new_position;
}
