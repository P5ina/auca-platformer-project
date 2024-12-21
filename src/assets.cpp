//
// Created by Timur Turatbekov on 07.12.2024.
//

#include "assets.h"

#include "raylib.h"

std::unique_ptr<AssetImages> load_images() {
    auto asset_images = std::make_unique<AssetImages>();

    asset_images->dialogue_panel_texture = std::make_unique<Texture2D>(
        LoadTexture(ASSETS_PATH"images/dialogue/panel.png")
    );
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

std::unique_ptr<AssetSounds> load_sounds() {
    InitAudioDevice();
    auto asset_sounds = std::make_unique<AssetSounds>();

    asset_sounds->jump_sound = std::make_unique<Sound>(LoadSound(ASSETS_PATH"sounds/jump.wav"));

    asset_sounds->main_menu_music = std::make_unique<Music>(
        LoadMusicStream(ASSETS_PATH"sounds/music/main_menu_music.mp3")
    );

    return asset_sounds;
}

void unload_sounds(std::unique_ptr<AssetSounds> asset_sounds) {
    UnloadSound(*asset_sounds->jump_sound);
    UnloadMusicStream(*asset_sounds->main_menu_music);
}

