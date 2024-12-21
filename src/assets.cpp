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
    asset_images->dialogue_indicator_texture = std::make_unique<Texture2D>(
        LoadTexture(ASSETS_PATH"images/dialogue/next_indicator.png")
    );
    asset_images->wall_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/wall/wall.png"));
    asset_images->air_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/air.png"));
    asset_images->player_texture = std::make_unique<Texture2D>(LoadTexture(ASSETS_PATH"images/player/player.png"));
    return asset_images;
}

// Transfers ownership and deallocates after leaving the scope
void unload_images(std::unique_ptr<AssetImages> asset_images) {
    UnloadTexture(*asset_images->dialogue_panel_texture);
    UnloadTexture(*asset_images->dialogue_indicator_texture);
    UnloadTexture(*asset_images->wall_texture);
    UnloadTexture(*asset_images->air_texture);
    UnloadTexture(*asset_images->player_texture);
}

std::unique_ptr<AssetSounds> load_sounds() {
    InitAudioDevice();
    auto asset_sounds = std::make_unique<AssetSounds>();

    asset_sounds->jump_sound = std::make_unique<Sound>(LoadSound(ASSETS_PATH"sounds/jump.mp3"));
    asset_sounds->dialogue_sound = std::make_unique<Sound>(LoadSound(ASSETS_PATH"sounds/dialogue_sound.mp3"));

    asset_sounds->main_menu_music = std::make_unique<Music>(
        LoadMusicStream(ASSETS_PATH"sounds/music/main_menu_music.mp3")
    );

    asset_sounds->dialogue_sound_pool = std::vector<Sound>(10);
    for (int i = 0; i < 10; i++) {
        asset_sounds->dialogue_sound_pool[i] = LoadSoundAlias(*asset_sounds->dialogue_sound);
    }

    return asset_sounds;
}

void unload_sounds(std::unique_ptr<AssetSounds> asset_sounds) {
    for (const auto & i : asset_sounds->dialogue_sound_pool) {
        UnloadSoundAlias(i);
    }

    UnloadSound(*asset_sounds->jump_sound);
    UnloadSound(*asset_sounds->dialogue_sound);
    UnloadMusicStream(*asset_sounds->main_menu_music);
}

