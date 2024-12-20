//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "assets.h"

#include "raylib.h"

std::unique_ptr<AssetImages> load_images() {
    auto asset_images = std::make_unique<AssetImages>();

    asset_images->dialogue_panel_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/dialogue/panel.png"));
    asset_images->wall_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/wall/wall.png"));
    asset_images->air_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/air.png"));
    asset_images->player_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/player/player.png"));
    return asset_images;
}

// Transfers ownership and deallocates after leaving the scope
void unload_images(std::unique_ptr<AssetImages> asset_images) {
    UnloadTexture(*asset_images->wall_texture);
    UnloadTexture(*asset_images->air_texture);
    UnloadTexture(*asset_images->player_texture);
}
