//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "level.h"

#include <format>
#include <global_state.h>
#include <graphics_old.h>
#include <iostream>
#include <string>
#include <utilities.h>
#include <gameobjects/player.h>

#include "box2d/box2d.h"

void load_level(std::unique_ptr<GameState> &game_state, LevelPosition position) {
    const std::string filepath_prefix = ASSETS_PATH"images/levels/";

    auto [x, y, z] = position;
    std::string filepath = std::format("{}level_{}_{}_{}.png", filepath_prefix, x, y, z);
    Image level_image = LoadImage(filepath.c_str());

    std::unique_ptr<Level> new_level = parse_level(&level_image);
    game_state->loaded_level = std::move(new_level);

    UnloadImage(level_image);

    derive_graphics_metrics_from_level(game_state->loaded_level);
}

void unload_level(std::unique_ptr<GameState> &game_state) {
    b2DestroyWorld(game_state->loaded_level->world_id);
    despawn_player(game_state);

    game_state->loaded_level = nullptr;
}

b2ChainDef create_wall_chain(Level *level, Vector2i start_position) {
    Vector2i next_position = start_position;

    const Vector2i LOOK_OFFSET[4] = {
        { -1, -1 },
        { 0, -1 },
        { 0, 0 },
        { -1, 0 },
    };
    const Vector2i MOVE_DIRECTION[4] = {
        { 0, -1 },
        { 1, 0 },
        { 0, 1 },
        { -1, 0 },
    };
    int direction = 1;

    b2ChainDef chain_def = b2DefaultChainDef();
    chain_def.isLoop = true;

    std::vector<Vector2i> points;

    do {
        points.push_back(next_position);
        Vector2i left_pos = vector2i_add(next_position, LOOK_OFFSET[direction]);
        Vector2i right_pos = vector2i_add(next_position, LOOK_OFFSET[(direction + 1) % 4]);
        bool has_left = false;
        bool has_right = false;

        if (auto left_tile = get_tile(level, left_pos)) {
            has_left = left_tile->type == LevelTileType::WALL;
        }
        if (auto right_tile = get_tile(level, right_pos)) {
            has_right = right_tile->type == LevelTileType::WALL;
        }

        if (has_left) {
            direction = (direction - 1 + 4) % 4;
        }
        else if (has_right) {
            // direction is the same
            // direction = direction;
        }
        else {
            direction = (direction + 1) % 4;
        }
        next_position = vector2i_add(next_position, MOVE_DIRECTION[direction]);
    } while (next_position.x != start_position.x || next_position.y != start_position.y);

    chain_def.count = points.size();
    b2Vec2* raw_points = new b2Vec2[points.size()];
    for (int i = 0; i < points.size(); i++) {
        raw_points[i] = b2Vec2 {
            static_cast<float>(points[i].x),
            static_cast<float>(points[i].y)
        };
    }
    chain_def.points = raw_points;
    return chain_def;
}

void create_level_collisions(Level *level) {
    int width = level->columns;
    int height = level->rows;

    std::vector<Vector2i> starting_points;
    std::vector<bool> visited_tiles(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto start_tile = get_tile(level, { x, y });
            if (start_tile->type != LevelTileType::WALL) {
                continue;
            }
            bool is_visited = visited_tiles[y * width + x];

            if (is_visited) {
                continue;
            }

            std::queue<Vector2i> next_position;
            next_position.push({x, y});

            while (!next_position.empty()) {
                auto new_position = next_position.front();
                next_position.pop();
                auto new_tile = get_tile(level, new_position);

                if (new_tile.has_value() &&
                    new_tile->type == LevelTileType::WALL &&
                    !visited_tiles[new_position.y * width + new_position.x]) {

                    visited_tiles[new_position.y * width + new_position.x] = true;

                    next_position.push({new_position.x + 1, new_position.y});
                    next_position.push({new_position.x, new_position.y - 1});
                    next_position.push({new_position.x - 1, new_position.y});
                    next_position.push({new_position.x, new_position.y + 1});
                    }
            }

            b2BodyDef wall_body_def = b2DefaultBodyDef();
            b2BodyId wall_body_id = b2CreateBody(level->world_id, &wall_body_def);

            b2ChainDef chain_def = create_wall_chain(level, { x, y });
            chain_def.filter.categoryBits = static_cast<uint32_t>(PhysicsCategories::WALL);

            b2CreateChain(wall_body_id, &chain_def);
            level->wall_bodies.push_back(wall_body_id);
        }
    }
}

std::unique_ptr<Level> parse_level(const Image *image) {
    const int width = image->width;
    const int height = image->height;

    b2WorldDef world_def = b2DefaultWorldDef();
    world_def.gravity = {0.0f, 20.0f};
    const b2WorldId world_id = b2CreateWorld(&world_def);

    std::vector<LevelTile> tiles(height * width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color tile_color = GetImageColor(*image, x, y);
            LevelTile tile = create_level_tile(tile_color, {x, y});
            tiles[y * width + x] = tile;
        }
    }

    auto level = std::make_unique<Level>(Level{
        .rows = height,
        .columns = width,
        .world_id = world_id,
        .tiles = tiles,
        .wall_bodies = std::vector<b2BodyId>()
    });

    // Calculate shapes for walls
    create_level_collisions(level.get());

    return level;
}

// Could be done using dictionaries
LevelTile create_level_tile(Color color, Vector2i position) {
    LevelTileType type = LevelTileType::AIR;

    if (ColorIsEqual(color, get_color_from_hex(AIR_COLOR))) {
        type = LevelTileType::AIR;
    }
    if (ColorIsEqual(color, get_color_from_hex(WALL_COLOR))) {
        type = LevelTileType::WALL;
    }

    return LevelTile{
        .position = position,
        .type = type,
    };
}

void set_tile(std::unique_ptr<Level> &level, LevelTile tile, Vector2i position) {
    set_tile(level, tile, position.x + position.y * level->columns);
}

void set_tile(std::unique_ptr<Level> &level, LevelTile tile, int tile_index) {
    level->tiles[tile_index] = tile;
}

std::optional<LevelTile> get_tile(Level *level, Vector2i position) {
    if (position.x < 0 || position.x >= level->columns ||
        position.y < 0 || position.y >= level->rows) {
        return {};
    }
    return level->tiles[position.x + position.y * level->columns];
}

std::optional<LevelTile> get_tile(Level *level, int tile_index) {
    if (tile_index < 0 || tile_index >= level->columns * level->rows)
        return {};
    return level->tiles[tile_index];
}

std::optional<int> get_tile_index(Level *level, Vector2i position) {
    if (position.x < 0 || position.x >= level->columns ||
        position.y < 0 || position.y >= level->rows) {
        return {};
    }
    return position.x + position.y * level->columns;
}

