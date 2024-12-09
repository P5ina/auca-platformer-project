//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "level.h"

#include <format>
#include <global_state.h>
#include <graphics_old.h>
#include <string>
#include <utilities.h>
#include <characters/player.h>

#include "box2d/box2d.h"

void load_level(std::unique_ptr<GameState> &game_state, LevelPosition position) {
    const std::string filepath_prefix = ASSETS_PATH"images/levels/";

    auto [x, y, z] = position;
    std::string filepath = std::format("{}level_{}_{}_{}.png", filepath_prefix, x, y, z);
    Image level_image = LoadImage(filepath.c_str());

    LoadedLevel new_level = parse_level(&level_image);
    game_state->loaded_level = std::make_unique<LoadedLevel>(new_level);

    UnloadImage(level_image);

    spawn_player(game_state);
    derive_graphics_metrics_from_level(game_state->loaded_level);
}

void unload_level(std::unique_ptr<GameState> &game_state) {
    b2DestroyWorld(game_state->loaded_level->world_id);
    despawn_player(game_state);

    game_state->loaded_level = nullptr;
}


LoadedLevel parse_level(const Image *image) {
    int width = image->width;
    int height = image->height;

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, -10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);

    std::vector<LevelTile> tiles(height * width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Color tile_color = GetImageColor(*image, x, y);
            LevelTileType tileType = create_tile_type(tile_color);
            tiles[y * width + x] = create_tile(worldId, tileType, { x, y });
        }
    }

    return LoadedLevel{
        height,
        width,
        worldId,
        tiles
    };
}

// Could be done using dictionaries
LevelTileType create_tile_type(Color color) {
    if (ColorIsEqual(color, get_color_from_hex(AIR_COLOR))) {
        return LevelTileType::AIR;
    }
    if (ColorIsEqual(color, get_color_from_hex(WALL_COLOR))) {
        return LevelTileType::WALL;
    }
    if (ColorIsEqual(color, get_color_from_hex(PLAYER_SPAWN_COLOR))) {
        return LevelTileType::PLAYER_SPAWN;
    }
    if (ColorIsEqual(color, get_color_from_hex(COIN_COLOR))) {
        return LevelTileType::COIN;
    }
    return LevelTileType::AIR;
}

LevelTile create_tile(b2WorldId world_id, LevelTileType type, Vector2i position) {
    switch (type) {
        case LevelTileType::WALL: {
            // TODO: Create static body
            b2BodyDef wall_body_def = b2DefaultBodyDef();
            wall_body_def.position = { position.x + 0.5f, position.y + 0.5f };
            b2BodyId wall_body_id = b2CreateBody(world_id, &wall_body_def);
            b2Polygon wall_box = b2MakeBox(0.5f, 0.5f);
            b2ShapeDef wall_shape_def = b2DefaultShapeDef();
            b2CreatePolygonShape(wall_body_id, &wall_shape_def, &wall_box);
            return LevelTile {
                type,
                wall_body_id
            };
        }
        case LevelTileType::PLAYER_SPAWN:
        case LevelTileType::AIR:
        case LevelTileType::COIN: {
            return {
                type,
                {}
            };
        }
    }
}

void set_tile(std::unique_ptr<LoadedLevel> &level, LevelTile tile, Vector2i position) {
    set_tile(level, tile, position.x + position.y * level->columns);
}

void set_tile(std::unique_ptr<LoadedLevel> &level, LevelTile tile, int tile_index) {
    level->tiles[tile_index] = tile;
}

std::optional<LevelTile> get_tile(std::unique_ptr<LoadedLevel> &level, Vector2i position) {
    if (position.x < 0 || position.x >= level->columns ||
        position.y < 0 || position.y >= level->rows) {
        return {};
    }
    return level->tiles[position.x + position.y * level->columns];
}
