//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef LEVEL_H
#define LEVEL_H

#include <optional>
#include <utilities.h>
#include <gameobjects/knowledge_book.h>
#include <nlohmann/adl_serializer.hpp>

#include "raylib.h"
#include "box2d/box2d.h"

const std::string AIR_COLOR = "#000000";
const std::string WALL_COLOR = "#ffffff";

struct LevelFileData {
    std::vector<KnowledgeBook> books;
};

enum class LevelTileType {
    AIR,
    WALL
};

struct LevelTile {
    Vector2i position;
    LevelTileType type;
};

struct Level {
    Vector2i position;
    int rows;
    int columns;
    b2WorldId world_id;
    std::vector<LevelTile> tiles;
    std::vector<b2BodyId> wall_bodies;
    std::vector<KnowledgeBook> books;
};


void to_json(nlohmann::json& j, const LevelFileData& d);
void from_json(const nlohmann::json& j, LevelFileData& d);

bool has_level(Vector2i position);
std::unique_ptr<Level> load_level(Vector2i position);
void unload_level(std::unique_ptr<Level> level);
std::unique_ptr<Level> parse_level(const Image *image, const nlohmann::json *json_data);
LevelTile create_level_tile(Color color, Vector2i position);

void set_tile(std::unique_ptr<Level> &level, LevelTile tile, Vector2i position);
void set_tile(std::unique_ptr<Level> &level, LevelTile tile, int tile_index);
std::optional<LevelTile> get_tile(Level *level, Vector2i position);
std::optional<LevelTile> get_tile(Level *level, int tile_index);
std::optional<int> get_tile_index(Level *level, Vector2i position);

void collect_book(Level* level, KnowledgeBook* book);

#endif //LEVEL_H
