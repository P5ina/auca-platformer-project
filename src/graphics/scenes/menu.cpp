//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "menu.h"
#include "raylib.h"

float main_menu_elapsed_time = 0.0f;

struct FireParticle {
    Vector2 position;
    float rotation;
    float rotationSpeed;
    float wobbleAmplitude;
    float wobbleFrequency;
    Color color;
};

Text game_title = {
    "Deep Woods",
    { 0.50f, 0.30f },
    100.0f,
};

void draw_menu() {
    main_menu_elapsed_time += GetFrameTime();
    draw_game_title();
}

void draw_game_title() {

}