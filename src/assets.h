//
// Created by Timur Turatbekov on 07.12.2024.
//

#ifndef ASSETS_H
#define ASSETS_H
#include <memory>

#include "raylib.h"

struct AssetImages {
    std::unique_ptr<Texture2D> dialogue_panel_texture;
    std::unique_ptr<Texture2D> wall_texture;
    std::unique_ptr<Texture2D> air_texture;
    std::unique_ptr<Texture2D> player_texture;
};

struct Assets {
    std::unique_ptr<AssetImages> images;
};

std::unique_ptr<AssetImages> load_images();
void unload_images(std::unique_ptr<AssetImages> asset_images);

#endif //ASSETS_H
