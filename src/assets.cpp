//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "assets.h"

#include "raylib.h"

void load_images(Assets* assets) {
    assets->images.wall_texture = LoadTexture(ASSETS_PATH"images/wall/wall.png");
    assets->images.air_texture = LoadTexture(ASSETS_PATH"images/air.png");
    assets->images.player_texture  = LoadTexture(ASSETS_PATH"images/player/player.png");
}

void unload_images(Assets* assets) {
    UnloadTexture(assets->images.wall_texture);
    UnloadTexture(assets->images.air_texture);
    UnloadTexture(assets->images.player_texture);
}

