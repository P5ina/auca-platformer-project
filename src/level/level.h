//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef LEVEL_H
#define LEVEL_H

#include <memory>

#include "raylib.h"

enum LevelTiles {
    AIR,
    WALL,
    PLAYER_SPAWN,
    COIN,
    EXIT
};

struct Level {
    size_t rows = 0, columns = 0;
    std::unique_ptr<LevelTiles[]> tiles = nullptr;
};

Level parse_level(Image *image);

#endif //LEVEL_H
