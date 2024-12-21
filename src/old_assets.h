#ifndef OLD_ASSETS_H
#define OLD_ASSETS_H

#include "raylib.h"
#include "globals.h"

void load_shaders() {
    crt_shader = LoadShader(nullptr, ASSETS_PATH"shaders/crt.frag");
    air_shader = LoadShader(nullptr, ASSETS_PATH"shaders/air.frag");
}

void unload_shaders() {
    UnloadShader(crt_shader);
    UnloadShader(air_shader);
}

void load_fonts() {
    menu_font = LoadFontEx(ASSETS_PATH"fonts/easvhs.ttf", 64, nullptr, 128);
}

void unload_fonts() {
    UnloadFont(menu_font);
}

void draw_image(Texture2D image, Vector2 pos, float size) {
    draw_image(image, pos, size, size);
}

void draw_image(Texture2D image, Vector2 pos, float width, float height) {
    Rectangle source = { 0.0f, 0.0f, static_cast<float>(image.width), static_cast<float>(image.height) };
    Rectangle destination = { pos.x, pos.y, width, height };
    DrawTexturePro(image, source, destination, { 0.0f, 0.0f }, 0.0f, WHITE);
}

void load_sounds() {
    InitAudioDevice();

    coin_sound = LoadSound(ASSETS_PATH"sounds/coin.wav");
    exit_sound = LoadSound(ASSETS_PATH"sounds/exit.wav");
    jump_sound = LoadSound(ASSETS_PATH"sounds/jump.wav");
}

void unload_sounds() {
    UnloadSound(coin_sound);
    UnloadSound(exit_sound);
    UnloadSound(jump_sound);
}

#endif // OLD_ASSETS_H
