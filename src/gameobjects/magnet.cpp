//
// Created by Timur Turatbekov on 22.12.2024.
//

#include "magnet.h"

#include <iostream>
#include <graphics/metrics.h>

#include "raylib.h"
#include "raymath.h"

struct MagnetOverlapContext {
    b2BodyId body_id;
};

bool magnet_overlap_callback(b2ShapeId shapeId, void *context) {
    auto magnet_context = static_cast<MagnetOverlapContext *>(context);
    b2BodyId body_id = b2Shape_GetBody(shapeId);
    magnet_context->body_id = body_id;
    std::cout << "Magnet overlap callback" << std::endl;
    return false;
}

void update_magnet_cursor(MagnetCursor *magnet_cursor, Level *level, GameInput *game_input) {
    float ui_scale = get_ui_scale();
    float cursor_size = 30.0f * ui_scale;
    float tile_size = get_tile_screen_size(level);

    float width = get_display_width();
    float height = get_display_height();

    if (B2_IS_NULL(magnet_cursor->attached_to)) {
        Vector2 cursor_pos = magnet_cursor->position;
        Vector2 mouse_delta = GetMouseDelta();
        float mouse_sens = 1.0f;

        cursor_pos = Vector2Add(cursor_pos, Vector2Scale(mouse_delta, mouse_sens));

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
        .center = { 0, 0 },
        .radius = (cursor_size / tile_size) / 2,
    };
    b2Transform query_transform;
    query_transform.p = cursor_level_position;
    query_transform.q = b2Rot_identity;
    b2QueryFilter query_filter = b2DefaultQueryFilter();
    query_filter.categoryBits = PhysicsLayers::PLAYER_LAYER;
    query_filter.maskBits = PhysicsLayers::MAGNET_BLOCK_LAYER;

    magnet_cursor->is_pull = game_input->magnet_pull;
    magnet_cursor->is_push = game_input->magnet_push;

    if ((magnet_cursor->is_pull || magnet_cursor->is_push) && B2_IS_NULL(magnet_cursor->attached_to)) {
        MagnetOverlapContext context{};
        b2World_OverlapCircle(level->world_id, &query_circle, query_transform, query_filter, magnet_overlap_callback, &context);
        magnet_cursor->attached_to = context.body_id;
        magnet_cursor->is_player_moving = true;
    }
    // Stop pulling
    if ((!magnet_cursor->is_pull && !magnet_cursor->is_push) && !B2_IS_NULL(magnet_cursor->attached_to)) {
        magnet_cursor->attached_to = b2_nullBodyId;
        magnet_cursor->is_player_moving = false;
    }
}

void draw_magnet_cursor(MagnetCursor *magnet_cursor, Assets *assets) {
    float ui_scale = get_ui_scale();
    float cursor_size = 26.0f * ui_scale;

    Texture2D texture = assets->images->magnet_cursor;

    float cursor_frame = 0;
    if (magnet_cursor->is_pull) {
        cursor_frame += 1;
    }
    if (magnet_cursor->is_push) {
        cursor_frame += 2;
    }

    Rectangle source = {8.0f * cursor_frame, 0, 8, 8};
    Rectangle dest = {
        magnet_cursor->position.x - cursor_size / 2, magnet_cursor->position.y - cursor_size / 2,
        cursor_size, cursor_size
    };

    if (magnet_cursor->is_pull) {
        draw_magnet_effect(magnet_cursor, assets->images->magnet_pulling_texture);
    }
    if (magnet_cursor->is_push) {
        draw_magnet_effect(magnet_cursor, assets->images->magnet_pushing_texture);
    }

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
}

void draw_magnet_effect(MagnetCursor *magnet_cursor, Texture2D texture) {
    int frame = static_cast<int>(GetTime() / 0.1) % 4;
    float effect_size = 95.0f * get_ui_scale();

    Rectangle source = {static_cast<float>(frame) * 16, 0, 16, 16};
    Rectangle dest = {
        magnet_cursor->position.x - effect_size / 2, magnet_cursor->position.y - effect_size / 2,
        effect_size, effect_size
    };

    DrawTexturePro(texture, source, dest, {0, 0}, 0, WHITE);
}
