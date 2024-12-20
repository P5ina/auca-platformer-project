//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef ASSETS_H
#define ASSETS_H
#include "raylib.h"

struct AssetImages {
    Texture2D dialogue_panel_texture;
    Texture2D wall_texture;
    Texture2D air_texture;
    Texture2D player_texture;
};

struct Assets {
    AssetImages images;
};

void load_images(Assets* assets);
void unload_images(Assets* assets);

#endif //ASSETS_H
