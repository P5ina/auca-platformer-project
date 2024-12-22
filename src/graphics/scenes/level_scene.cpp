//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "level_scene.h"

#include <globals.h>
#include <graphics/metrics.h>
#include <level/tiles/air.h>
#include <level/tiles/magnet_block.h>
#include <level/tiles/wall.h>

#include "raymath.h"
#include "box2d/box2d.h"

// void draw_game_overlay() {
//     Text score = {
//         "Score " + std::to_string(player_score),
//         { 0.50f, 0.05f },
//         48.0f
//     };
//     Text score_shadow = {
//         "Score " + std::to_string(player_score),
//         { 0.503f, 0.055f },
//         48.0f,
//         GRAY
//     };
//
//     draw_text(score_shadow);
//     draw_text(score);
// }

void draw_level(Level *level, Assets *assets) {
    float size = get_tile_screen_size(level);

    for (int row = 0; row < level->rows; ++row) {
        for (int column = 0; column < level->columns; ++column) {
            Vector2 pos = get_tile_screen_position(level, {
                static_cast<float>(column), static_cast<float>(row)
            });

            LevelTile tile = level->tiles[row * level->columns + column];

            switch (tile.type) {
                case LevelTileType::AIR: {
                    draw_air(pos, size, assets);
                    break;
                }
                case LevelTileType::WALL: {
                    char surroundings = get_surroundings(level, column, row);
                    draw_wall(pos, size, surroundings, assets);
                    break;
                }
                case LevelTileType::MAGNET_BLOCK: {
                    draw_magnet_block(pos, size, assets);
                    break;
                }
            }
        }
    }

    for (int i = 0; i < level->books.size(); i++) {
        draw_book(level, &level->books[i], assets);
    }
}

void draw_level_debug_overlay(Level *level) {
    for (auto body : level->wall_bodies) {
        b2ShapeId* shapes = new b2ShapeId[1000];
        int size = b2Body_GetShapes(body, shapes, 1000);
        for (int i = 0; i < size; i++) {
            auto segment = b2Shape_GetSmoothSegment(shapes[i]);

            Vector2 pos1 = get_tile_screen_position(level, {
                segment.segment.point1.x, segment.segment.point1.y
            });
            Vector2 pos2 = get_tile_screen_position(level, {
                segment.segment.point2.x, segment.segment.point2.y
            });

            Vector2 dir = Vector2Subtract(pos2, pos1);
            Vector2 center = { (pos1.x + pos2.x) / 2.0f, (pos1.y + pos2.y) / 2.0f };
            Vector2 n = { pos2.y - pos1.y, pos1.x - pos2.x };

            DrawLineEx(pos1, pos2, 1, RED);
            DrawLineEx(center, Vector2Add(center, Vector2Scale(n, 0.5f)), 1.0f, BLUE);

            Vector2 tri1 = Vector2Add(Vector2Subtract(pos2, Vector2Scale(dir, 0.2f)), Vector2Scale(n, -0.1f));
            Vector2 tri3 = Vector2Add(Vector2Subtract(pos2, Vector2Scale(dir, 0.2f)), Vector2Scale(n, 0.1f));
            DrawTriangle(tri1, pos2, tri3, RED);
        }
    }

    for (int i = 0; i < level->books.size(); i++) {
        draw_book_debug_overlay(level, &level->books[i]);
    }
}


void draw_player(Level* level, Player *player, Assets *assets) {
    if (player == nullptr)
        return;

    b2Vec2 player_pos = b2Body_GetPosition(player->body_id);

    Vector2 pos = get_tile_screen_position(level, {
        player_pos.x - 0.5f, player_pos.y - 0.5f
    });
    float size = get_tile_screen_size(level);

    Rectangle destination = { pos.x, pos.y, size, size };
    int frame_x = 0;
    int frame_y = 0;

    const int frames_count = 6;
    const double frame_time = 0.1;

    if (player->jump_timer > frames_count * frame_time) {
        if (player->is_facing_left) {
            frame_y = 1;
        } else {
            frame_y = 0;
        }
    }
    else {
        double time = player->jump_timer;
        frame_x = std::min(static_cast<int>(time / frame_time), frames_count - 1);
        if (player->is_facing_left) {
            frame_y = 3;
        } else {
            frame_y = 2;
        }
    }
    Rectangle source = { frame_x * 8.0f, frame_y * 8.0f, 8.0f, 8.0f };
    DrawTexturePro(*assets->images->player_texture, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void draw_player_debug_overlay(Level *level, Player *player) {
    if (player == nullptr)
        return;

    b2Vec2 player_pos = b2Body_GetPosition(player->body_id);

    auto *shapes = new b2ShapeId[1];
    b2Body_GetShapes(player->body_id, shapes, 1);

    b2Capsule capsule = b2Shape_GetCapsule(shapes[0]);

    Vector2 pos1 = get_tile_screen_position(level, {
        player_pos.x + capsule.center1.x, player_pos.y + capsule.center1.y
    });
    Vector2 pos2 = get_tile_screen_position(level, {
        player_pos.x + capsule.center2.x, player_pos.y + capsule.center2.y
    });
    float size = capsule.radius * get_tile_screen_size(level);

    DrawCircleV(pos1, size, Fade(GREEN, 0.5f));
    DrawCircleV(pos2, size, Fade(GREEN, 0.5f));
}


void draw_book(Level* level, KnowledgeBook* book, Assets* assets) {
    if (book->collected)
        return;

    b2Vec2 book_pos = b2Body_GetPosition(book->body_id);

    Vector2 pos = get_tile_screen_position(level, {
        book_pos.x - 0.5f, book_pos.y - 0.5f
    });
    float size = get_tile_screen_size(level);

    Rectangle destination = { pos.x, pos.y, size, size };
    Rectangle source = {
        0, 0,
        static_cast<float>(assets->images->book_texture.width),
        static_cast<float>(assets->images->book_texture.height)
    };

    DrawTexturePro(assets->images->book_texture, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void draw_book_debug_overlay(Level *level, KnowledgeBook *book) {
    if (book->collected)
        return;

    b2Vec2 book_pos = b2Body_GetPosition(book->body_id);

    Vector2 pos = get_tile_screen_position(level, {
        book_pos.x, book_pos.y
    });
    float size = get_tile_screen_size(level);

    DrawCircleV(pos, size / 2, Fade(GREEN, 0.2f));
}

